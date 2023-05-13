// ITMO 09.03.02 2020
// Artem Mikheev M3101
// Apache License
#ifndef ID3V2_H
#define ID3V2_H

#include <stdint.h>

/**
 * Synchsafe integer as defined by the ID3v2 spec. If the uppermost bit of one of these bytes isn't a zero,
 * we will throw an error and won't continue parsing.
 */
typedef uint32_t synchsafe_uint32;

/**
 * Converts a uint32 to a synchsafe version which can then be written as bytes to a file. If the value can't be converted to a
 * 4-byte synchsafe, we throw an error.
 * @param value a 32-bit unsigned integer
 * @return the synchsafe version of that integer
 */
synchsafe_uint32 uint32_to_synchsafe(uint32_t value);

/**
 * Converts a synchsafe 32-bit uint which was read from a file to a uint32.
 * This will throw an error if the passed value isn't a valid synchsafe.
 * @param value a 32-bit synchsafe integer
 * @return the normal uint32 version of that synchsafe
 */
uint32_t synchsafe_to_uint32(synchsafe_uint32 value);

/**
 * Structure defining the header of the ID3v2 flag. We do not pack this structure to make sure that reading and writing
 * is speedy on all architectures - it's better to parse every field one by one.
 */
typedef struct id3v2_header {
	char identifier[3];
	uint16_t version;
	uint8_t flags;
	uint32_t size;
} id3v2_header;

/**
 * Reads an id3v2 header from the memory location pointed to by src into the preallocated header. Returns
 * the next byte after the parsed header.
 */
unsigned char *read_header(unsigned char *src, id3v2_header *header);

/**
 * Writes a header into the memory location pointed to by dest, returns the byte after the end of the header.
 */
unsigned char *write_header(unsigned char *dest, id3v2_header *header);



/**
 * Structure defining the extended header of the ID3v2 flag. This defines the actual data of the extended header, we
 * use it simply to read the extended header and then parse the actual data into id3v2_extended_header.
 */
typedef struct id3v2_extended_header_b {
	uint32_t size;
	uint8_t n_flag_bytes;	// must be 1
	uint8_t flags;
} id3v2_extended_header_b;

/**
 * Enumeration defining the possible flags in the extended header. These are the only ones in the standard and the
 * only ones we support.
 */
typedef enum id3v2_extended_header_flag_type {
	UPDATE, CRC, RESTRICTIONS
} id3v2_extended_header_flag_type;

/**
 * Structure defining all of the flags in the extended header. The length of data depends on flag_data_length.
 */
typedef struct id3v2_extended_header_flag_node {
	id3v2_extended_header_flag_type type;
	struct id3v2_extended_header_flag_node *next;
	uint8_t data_length;
	unsigned char *data;
} id3v2_extended_header_flag_node;

/**
 * The actual extended header which we parsed from the bytes.
 */
typedef struct id3v2_extended_header {
	id3v2_extended_header_b extended_header_b;
	id3v2_extended_header_flag_node *flag_list;
} id3v2_extended_header;

/**
 * Reads an extended header from the memory location pointed to by src into the preallocated extended_header. Returns
 * the next byte after the parsed header.
 */
unsigned char *read_extended_header(unsigned char *src, id3v2_extended_header *extended_header);

/**
 * Writes an extended header into the memory location pointed to by dest, returns the byte after the end of the extended header.
 */
unsigned char *write_extended_header(unsigned char *dest, id3v2_extended_header *extended_header);



/**
 * Structure defining the footer of the id3v2 tag. If the flags here aren't valid, or the data doesn't match the header, we
 * throw an error. This is mostly so that we can quickly find tags which are appended to the MP3 file.
 */
typedef struct id3v2_footer {
	char identifier[3];
	uint16_t version;
	uint8_t flags;
	uint32_t size;
} id3v2_footer;

/**
 * Reads an id3v2 footer from the memory location pointed to by src into the preallocated footer. Returns
 * the next byte after the parsed footer.
 */
unsigned char *read_footer(unsigned char *src, id3v2_footer *footer);

/**
 * Writes a footer into the memory location pointed to by dest, returns the byte after the end of the footer.
 */
unsigned char *write_footer(unsigned char *dest, id3v2_footer *footer);



/**
 * Structure defining the actual representation of a frame in the data.
 */
typedef struct id3v2_frame_b {
	char id[4];
	uint32_t size;
	uint8_t status_flags;
	uint8_t format_flags;
} id3v2_frame_b;

typedef struct id3v2_frame {
	// Currently these don't affect the actual operation of the program, so they are pretty much ignored. However
	// if the read only flag is set we raise an error, other ones are just ignored.
	struct {
		int tag_alter_preservation;
		int file_alter_preservation;
		int read_only;
	} status_flags;
	// Currently we do not support any of these except unsynchronization, so tags with unknown flags
	// will be ignored (because there's no way to read a compressed, encrypted etc tag).
	struct {
		int grouping_identity;
		int compression;
		int encryption;
		int unsynchronized;
		int data_length;
	} format_flags;
	id3v2_frame_b frame_b;
	struct id3v2_frame *next_frame;
	// The actual data stored in the tag
	unsigned char *data;
} id3v2_frame;

/**
 * Reads an id3v2 frame from the memory location pointed to by src into the preallocated frame. Returns
 * the next byte after the parsed frame.
 */
unsigned char *read_frame(unsigned char *src, id3v2_frame *frame);

/**
 * Writes a frame into the memory location pointed to by dest, returns the byte after the end of the frame.
 */
unsigned char *write_frame(unsigned char *dest, id3v2_frame *frame);



/**
 * Structure which we define as an information/data storage which we update while we parse the flags. We do not read actual
 * data into this structure, but instead into the respective tag parts themselves and then fill this for easier use.
 */
typedef struct id3v2_tag {
	/**
	 * The flags which are parsed from the header and footer.
	 */
	struct {
		int unsynchronized;
		int extended_header_present;
		int experimental;
		int footer_present;
	} flags;
	id3v2_header header;
	id3v2_extended_header extended_header;
	id3v2_frame *frame_list;
	id3v2_footer footer;
	// The actual size taken up by extended header + frames, so that we know if we have space left
	// in the form of padding in order to use it up. This should be initialized during reading the tag,
	// and updated when the frames are edited.
	uint32_t actual_size;
} id3v2_tag;

/**
 * Reads an id3v2 tag starting at src into the preallocated tag argument. Yes, technically tags can also be read from
 * the footer, but since we currently only support finding the tags from the front this should do as well.
 */
unsigned char *read_tag(unsigned char *src, id3v2_tag *tag);

/**
 * Writes an id3v2 tag into the memory pointed to by dest. This doesn't do any checking and relies on getting
 * a valid tag, which should make sense since we have to update the tag with every change anyway.
 */
unsigned char *write_tag(unsigned char *dest, id3v2_tag *tag);

/**
 * Function which should return the full amount of memory which this tag takes up.
 */
uint32_t get_full_tag_size(id3v2_tag *tag);


#endif //ID3V2_H