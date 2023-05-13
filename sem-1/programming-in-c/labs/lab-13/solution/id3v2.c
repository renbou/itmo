// ITMO 09.03.02 2020
// Artem Mikheev M3101
// Apache License

#include <stdlib.h>
#include "id3v2.h"
#include "utils.h"
#include "errors.h"

synchsafe_uint32 uint32_to_synchsafe(uint32_t value) {
	// we can only store 28 bits inside a 32-bit synchsafe integer
	if (value >= (1 << 28)) {
		FATAL("Unable to convert uint32 to a synchsafe integer, meaning that writing data would fail.");
	}
	synchsafe_uint32 result = 0;
	result |= (((value >> 0) & 0x7f) << 0);
	result |= (((value >> 7) & 0x7f) << 8);
	result |= (((value >> 14) & 0x7f) << 16);
	result |= (((value >> 21) & 0x7f) << 24);
	return result;
}

uint32_t synchsafe_to_uint32(synchsafe_uint32 value) {
	uint32_t result = 0;
	result |= (((value >> 0) & 0xff) << 0);
	result |= (((value >> 8) & 0xff) << 7);
	result |= (((value >> 16) & 0xff) << 14);
	result |= (((value >> 24) & 0xff) << 21);
	return result;
}

unsigned char *read_header(unsigned char *src, id3v2_header *header) {
	if (src == NULL) {
		FATAL("Unable to read an id3v2 header from a NULL pointer.");
	}
	if (header == NULL) {
		FATAL("Unable to read an id3v2 header into a NULL pointer.");
	}

	memcpy(header->identifier, src, 3);
	if (strncmp(header->identifier, "ID3", 3) != 0) {
		FATAL("Unable to parse a non-id3v2 header (magic header identifier isn't \'ID3\')");
	}

	header->version = read_bigendian_u16(src + 3);
	if (upper16(header->version) != 4 || lower16(header->version) != 0) {
		FATAL("Currently we can only parse tags of version 2.4, but encountered a different version.");
	}

	header->flags = read_u8(src + 5);
	header->size = synchsafe_to_uint32(read_bigendian_u32(src+6));
	return src + 10;
}

unsigned char *write_header(unsigned char *dest, id3v2_header *header) {
	if (dest == NULL) {
		FATAL("Unable to write an id3v2 header to a NULL pointer.");
	}
	if (header == NULL) {
		FATAL("Unable to write null id3v2 header.");
	}

	memcpy(dest, header->identifier, 3);
	write_bigendian_u16(header->version, dest + 3);
	write_u8(header->flags, dest + 5);
	write_bigendian_u32(uint32_to_synchsafe(header->size), dest + 6);
	return dest + 10;
}

unsigned char *read_extended_header(unsigned char *src, id3v2_extended_header *extended_header) {
	if (src == NULL) {
		FATAL("Unable to read an id3v2 extended header from a NULL pointer.");
	}
	if (extended_header == NULL) {
		FATAL("Unable to read an id3v2 extended header into a NULL pointer.");
	}

	extended_header->extended_header_b.size = read_bigendian_u32(src);
	if (extended_header->extended_header_b.size < 6) {
		FATAL("Invalid size of extended header found (less than the header itself).");
	}

	extended_header->extended_header_b.n_flag_bytes = read_u8(src + 4);
	if (extended_header->extended_header_b.n_flag_bytes != 1) {
		FATAL("Unsupported length of the extended header flags (only 1-byte flags supported).");
	}

	extended_header->extended_header_b.flags = read_u8(src + 5);
	extended_header->flag_list = NULL;

	unsigned char *extended_header_flag_data = src + 6;
	id3v2_extended_header_flag_node *last_node = NULL;
	if (extended_header->extended_header_b.flags & (1 << 6)) {
		id3v2_extended_header_flag_node *update_flag =
			(id3v2_extended_header_flag_node *)xalloc(sizeof(id3v2_extended_header_flag_node));
		update_flag->type = UPDATE;
		update_flag->data_length = read_u8(extended_header_flag_data);
		if (update_flag->data_length != 0) {
			FATAL("Invalid length of UPDATE flag in the extended header - probably wrong ordering of flags.");
		}
		update_flag->data = NULL;
		update_flag->next = NULL;

		extended_header_flag_data++;

		last_node = extended_header->flag_list = update_flag;
	}

	if (extended_header->extended_header_b.flags & (1 << 5)) {
		id3v2_extended_header_flag_node *crc_flag =
			(id3v2_extended_header_flag_node *)xalloc(sizeof(id3v2_extended_header_flag_node));
		crc_flag->type = CRC;
		crc_flag->data_length = read_u8(extended_header_flag_data);
		if (crc_flag->data_length != 5) {
			FATAL("Invalid length of CRC flag in the extended header - probably wrong ordering of flags.");
		}
		crc_flag->data = xalloc(5);
		crc_flag->next = NULL;
		memcpy(crc_flag->data, extended_header_flag_data + 1, 5);

		extended_header_flag_data += 6;

		if (last_node == NULL) {
			extended_header->flag_list = crc_flag;
		} else {
			last_node->next = crc_flag;
		}
		last_node = crc_flag;
	}

	if (extended_header->extended_header_b.flags & (1 << 4)) {
		id3v2_extended_header_flag_node *restrictions_flag =
			(id3v2_extended_header_flag_node *)xalloc(sizeof(id3v2_extended_header_flag_node));
		restrictions_flag->type = RESTRICTIONS;
		restrictions_flag->data_length = read_u8(extended_header_flag_data);
		if (restrictions_flag->data_length != 1) {
			FATAL("Invalid length of RESTRICTIONS flag in the extended header - probably wrong ordering of flags.");
		}
		restrictions_flag->data = xalloc(1);
		restrictions_flag->next = NULL;
		memcpy(restrictions_flag->data, extended_header_flag_data + 1, 1);

		extended_header_flag_data += 2;

		if (last_node == NULL) {
			extended_header->flag_list = restrictions_flag;
		} else {
			last_node->next = restrictions_flag;
		}
		last_node = restrictions_flag;
	}

	if (extended_header->extended_header_b.size != (extended_header_flag_data - src)) {
		FATAL("Invalid size of the extended header - got different length of actual data for all of the flags.");
	}

	return extended_header_flag_data;
}

unsigned char *write_extended_header(unsigned char *dest, id3v2_extended_header *extended_header) {
	if (dest == NULL) {
		FATAL("Unable to write an id3v2 extended header to a NULL pointer.");
	}
	if (extended_header == NULL) {
		FATAL("Unable to write null id3v2 extender header.");
	}

	write_bigendian_u32(uint32_to_synchsafe(extended_header->extended_header_b.size), dest);
	write_u8(extended_header->extended_header_b.n_flag_bytes, dest + 4);
	write_u8(extended_header->extended_header_b.flags, dest + 5);

	id3v2_extended_header_flag_node *node = extended_header->flag_list;
	unsigned char *extended_header_flag_data = dest + 6;
	while (node != NULL) {
		write_u8(node->data_length, extended_header_flag_data);
		if (node->data_length != 0) {
			if (node->data == NULL) {
				FATAL("Unable to write id3v2 extender header flags with NULL data.");
			}
			memcpy(extended_header_flag_data + 1, node->data, node->data_length);
		}
		extended_header_flag_data += 1 + node->data_length;
		node = node->next;
	}

	return extended_header_flag_data;
}

unsigned char *read_footer(unsigned char *src, id3v2_footer *footer) {
	if (src == NULL) {
		FATAL("Unable to read an id3v2 footer from a NULL pointer.");
	}
	if (footer == NULL) {
		FATAL("Unable to read an id3v2 footer into a NULL pointer.");
	}

	memcpy(footer->identifier, src, 3);
	if (strncmp(footer->identifier, "3DI", 3) != 0) {
		FATAL("Unable to parse a non-id3v2 footer (magic footer identifier isn't \'3DI\')");
	}

	footer->version = read_bigendian_u16(src + 3);
	if (upper16(footer->version) != 4 || lower16(footer->version) != 0) {
		FATAL("Currently we can only parse tags of version 2.4, but encountered a different version.");
	}

	footer->flags = read_u8(src + 5);
	footer->size = synchsafe_to_uint32(read_bigendian_u32(src+6));
	return src + 10;
}

unsigned char *write_footer(unsigned char *dest, id3v2_footer *footer) {
	if (dest == NULL) {
		FATAL("Unable to write an id3v2 footer to a NULL pointer.");
	}
	if (footer == NULL) {
		FATAL("Unable to write null id3v2 footer.");
	}

	memcpy(dest, footer->identifier, 3);
	write_bigendian_u16(footer->version, dest + 3);
	write_u8(footer->flags, dest + 5);
	write_bigendian_u32(uint32_to_synchsafe(footer->size), dest + 6);
	return dest + 10;
}

unsigned char *read_frame(unsigned char *src, id3v2_frame *frame) {
	if (src == NULL) {
		FATAL("Unable to read an id3v2 frame from a NULL pointer.");
	}
	if (frame == NULL) {
		FATAL("Unable to read an id3v2 frame into a NULL pointer.");
	}

	memcpy(frame->frame_b.id, src, 4);
	for (uint32_t i = 0; i < 4; i++) {
		if ((frame->frame_b.id[i] < 'A' || frame->frame_b.id[i] > 'Z') &&
			(frame->frame_b.id[i] < '0' || frame->frame_b.id[i] > '9')) {
			FATAL("Invalid id3v2 frame id, containing chars not fitting to A-Z0-9.");
		}
	}
	frame->frame_b.size = synchsafe_to_uint32(read_bigendian_u32(src + 4));

	// parse the frame status flags
	frame->frame_b.status_flags = read_u8(src + 8);
	frame->status_flags.tag_alter_preservation = (frame->frame_b.status_flags & (1 << 6));
	frame->status_flags.file_alter_preservation = (frame->frame_b.status_flags & (1 << 5));
	frame->status_flags.read_only = (frame->frame_b.status_flags & (1 << 4));

	// parse the frame format flags
	frame->frame_b.format_flags = read_u8(src + 9);
	frame->format_flags.grouping_identity = (frame->frame_b.format_flags & (1 << 6));
	frame->format_flags.compression = (frame->frame_b.format_flags & (1 << 3));
	frame->format_flags.encryption = (frame->frame_b.format_flags & (1 << 2));
	frame->format_flags.unsynchronized = (frame->frame_b.format_flags & (1 << 1));
	frame->format_flags.data_length = (frame->frame_b.format_flags & (1 << 0));

	// we can now read the data
	frame->data = NULL;
	if (frame->frame_b.size != 0) {
		frame->data = (unsigned char *) xalloc(frame->frame_b.size);
		memcpy(frame->data, src + 10, frame->frame_b.size);
	}

	frame->next_frame = NULL;
	return src + 10 + frame->frame_b.size;
}

unsigned char *write_frame(unsigned char *dest, id3v2_frame *frame) {
	if (dest == NULL) {
		FATAL("Unable to write an id3v2 frame to a NULL pointer.");
	}
	if (frame == NULL) {
		FATAL("Unable to write null id3v2 frame.");
	}

	memcpy(dest, frame->frame_b.id, 4);
	write_bigendian_u32(uint32_to_synchsafe(frame->frame_b.size), dest + 4);
	write_u8(frame->frame_b.status_flags, dest + 8);
	write_u8(frame->frame_b.format_flags, dest + 9);
	if (frame->frame_b.size != 0) {
		if (frame->data == NULL) {
			FATAL("Unable to write id3v2 extender header flags with NULL data.");
		}
		memcpy(dest + 10, frame->data, frame->frame_b.size);
	}
	return dest + 10 + frame->frame_b.size;
}

unsigned char *read_tag(unsigned char *src, id3v2_tag *tag) {
	if (src == NULL) {
		FATAL("Unable to read an id3v2 tag from a NULL pointer.");
	}
	if (tag == NULL) {
		FATAL("Unable to read an id3v2 tag into a NULL pointer.");
	}
	// zero out the tag completely so that we definitely don't mess up
	memset(tag, 0, sizeof(id3v2_tag));

	unsigned char *next_src = read_header(src, &tag->header);
	tag->flags.unsynchronized = (tag->header.flags & (1 << 7));
	tag->flags.extended_header_present = (tag->header.flags & (1 << 6));
	tag->flags.experimental = (tag->header.flags & (1 << 5));
	tag->flags.footer_present = (tag->header.flags & (1 << 4));

	if (tag->flags.extended_header_present) {
		next_src = read_extended_header(next_src, &tag->extended_header);
	}

	tag->frame_list = NULL;
	id3v2_frame *last_frame = NULL;
	while(next_src < src + 10 + tag->header.size && ((unsigned char *)next_src)[0] != '\0') {
		id3v2_frame *frame = (id3v2_frame *)xalloc(sizeof(id3v2_frame));
		next_src = read_frame(next_src, frame);
		if (tag->frame_list == NULL) {
			tag->frame_list = frame;
		} else {
			last_frame->next_frame = frame;
		}
		last_frame = frame;
	}

	if (next_src > src + 10 + tag->header.size) {
		FATAL("Invalid size of tag specified, actual frames length is bigger.");
	}

	tag->actual_size = next_src - (src + 10);
	next_src = src + 10 + tag->header.size;

	if (tag->flags.footer_present) {
		next_src = read_footer(next_src, &tag->footer);
	}
	return next_src;
}

unsigned char *write_tag(unsigned char *dest, id3v2_tag *tag) {
	if (dest == NULL) {
		FATAL("Unable to write an id3v2 tag to a NULL pointer.");
	}
	if (tag == NULL) {
		FATAL("Unable to write null id3v2 tag.");
	}

	unsigned char *next_dest = write_header(dest, &tag->header);
	if (tag->flags.extended_header_present) {
		next_dest = write_extended_header(next_dest, &tag->extended_header);
	}

	id3v2_frame *current_frame = tag->frame_list;
	while (current_frame != NULL) {
		next_dest = write_frame(next_dest, current_frame);
		current_frame = current_frame->next_frame;
	}

	next_dest = dest + 10 + tag->header.size;

	if (tag->flags.footer_present) {
		next_dest = write_footer(next_dest, &tag->footer);
	}
	return next_dest;
}

uint32_t get_full_tag_size(id3v2_tag *tag) {
	if (tag == NULL) {
		FATAL("Unable to get size of null id3v2 tag.");
	}
	uint32_t size = 10 + tag->header.size;
	if (tag->flags.footer_present) {
		size += 10;
	}
	return size;
}