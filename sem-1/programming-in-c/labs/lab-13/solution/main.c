// ITMO 09.03.02 2020
// Artem Mikheev M3101
// Apache License

#include <stdio.h>
#include "id3v2.h"
#include "config.h"
#include "utils.h"

/**
 * Local helper function which is used to print out one frame with respect to our table format.
 * @param frame
 */
void print_frame(id3v2_frame *frame) {
	if (frame->frame_b.size != 0) {
		printf("%.4s  | %9u | ",
			   frame->frame_b.id,
			   (uint32_t)frame->data[0]);
		const unsigned char *current_data = frame->data + 1;
		int first = 1;
		while(current_data < frame->data + frame->frame_b.size) {
			if (!first) {
				printf("| %s ", current_data);
			} else {
				printf("%s ", current_data);
				first = 0;
			}
			current_data += strlen((const char *)current_data) + 1;
		}
		puts("");
	} else {
		printf("%.4s  |   NONE    | NULL\n",
			   frame->frame_b.id);
	}
}

void show_all(id3v2_tag *tag) {
	printf("FRAME | TEXT TYPE | VALUES\n");
	id3v2_frame *current_frame = tag->frame_list;
	while (current_frame != NULL) {
		print_frame(current_frame);
		current_frame = current_frame->next_frame;
	}
}

void get_frame(id3v2_tag *tag, const char *name) {
	id3v2_frame *current_frame = tag->frame_list;

	int found = 0;
	while (current_frame != NULL) {
		if (strncmp(current_frame->frame_b.id, name, 4) == 0) {
			if (!found) {
				printf("FRAME | TEXT TYPE | VALUES\n");
				found = 1;
			}
			print_frame(current_frame);
		}
		current_frame = current_frame->next_frame;
	}
	if (!found) {
		printf("%s frame could not be found in the tag!\n", name);
	}
}

/**
 * Helper function to check if data of size size can fit into the padding, if it does, then simply update the padding,
 * otherwise set the padding to zero and update the tag size.
 */
void fit_into_padding(id3v2_tag *tag, uint32_t size) {
	// if we don't have enough padding to fit this value
	uint32_t padding = tag->header.size - tag->actual_size;
	if (size > padding) {
		tag->header.size += (size - padding);
		tag->actual_size = tag->header.size;
		if (tag->flags.footer_present) {
			tag->footer.size = tag->header.size;
		}
	} else {
		// otherwise we can simply reduce the size of the padding, we don't need to update the size of the tag
		tag->actual_size += size;
		// we don't need to change the size of the tag in the header, since this data fits in the padding
	}
}

void set_frame(id3v2_tag *tag, const char *name, const char *value) {
	id3v2_frame *current_frame = tag->frame_list;
	while (current_frame != NULL) {
		// stop once we found the frame
		if (strncmp(current_frame->frame_b.id, name, 4) == 0) {
			break;
		}
		current_frame = current_frame->next_frame;
	}

	const uint32_t value_size = strlen(value);
	const uint32_t new_frame_size = value_size + 1;

	// the frame wasn't found so we need to make a new one
	if (current_frame == NULL) {
		current_frame = (id3v2_frame *)xalloc(sizeof(id3v2_frame));
		memset(current_frame, 0, sizeof(id3v2_frame));
		memcpy(current_frame->frame_b.id, name, 4);
		// find the last frame and place this frame after it
		if (tag->frame_list == NULL) {
			tag->frame_list = current_frame;
		} else {
			id3v2_frame *last_frame = tag->frame_list;
			while (last_frame->next_frame != NULL) {
				last_frame = last_frame->next_frame;
			}
			last_frame->next_frame = current_frame;
		}
		// frame header is 10 bytes in size, plus the actual data
		fit_into_padding(tag, 10 + new_frame_size);
	} else {
		if (current_frame->status_flags.read_only) {
			FATAL("Frame is marked as read only, we have no right to change its value.");
		}
		// we already have the header, just need to update the size
		fit_into_padding(tag, new_frame_size - current_frame->frame_b.size);
	}

	if (current_frame->frame_b.size != 0) {
		free(current_frame->data);
	}
	current_frame->frame_b.size = new_frame_size;
	current_frame->data = NULL;
	if (new_frame_size != 0) {
		current_frame->data = (unsigned char *)xalloc(new_frame_size);
		current_frame->data[0] = 0;
		memcpy(current_frame->data + 1, value, value_size);
	}
}

int main(int argc, char *argv[]) {
	initialize_global_config(argc, argv);
	ID3v2_Config *config = get_global_config();

	// Open the requested file and get its size so that we can read all of the data
	FILE *mp3file = fopen(config->filename, "rb");
	fseek(mp3file, 0, SEEK_END);
	uint32_t file_size = ftell(mp3file);
	fseek(mp3file, 0, SEEK_SET);

	// Read the file data. We will parse a tag from here, and know the start of the actual data, so that when we are
	// writing a changed tag, we can first write the new tag and then write the data starting from the previously
	// saved point.
	unsigned char *data = xalloc(file_size);
	fread(data, sizeof(unsigned char), file_size, mp3file);
	fclose(mp3file);

	id3v2_tag *tag = (id3v2_tag *)xalloc(sizeof(id3v2_tag));
	unsigned char* data_start = read_tag(data, tag);

	// if we simply need to show or get a frame then there's no pointing in writing back a new tag
	if (config->running_mode == SHOW) {
		show_all(tag);
	} else if (config->running_mode == GET) {
		get_frame(tag, config->frame_name);
	} else {
		set_frame(tag, config->frame_name, config->frame_value);
		// now we need to write the new tag
		uint32_t tag_size = get_full_tag_size(tag);
		unsigned char *new_data = xalloc(tag_size);
		write_tag(new_data, tag);

		// write the data to the mp3 file
		mp3file = fopen(config->filename, "wb");
		fwrite(new_data, sizeof(unsigned char), tag_size, mp3file);
		fwrite(data_start, sizeof(unsigned char), file_size - (data_start - data), mp3file);
		fclose(mp3file);

		free(new_data);
	}

	free(data);
}