// ITMO 09.03.02 2020
// Artem Mikheev M3101
// Apache License
#ifndef ID3V2_CONFIG_H
#define ID3V2_CONFIG_H

typedef enum {SHOW, GET, SET} ID3v2_mode;

typedef struct {
	// Whether we are running on little or big endian, so that we write correct values
	int is_little_endian;
	// The file with which we will be operating. This is needed for all running modes.
	const char *filename;
	// The mode in which the program was launched (either show all tags, get one tag or set one tag)
	ID3v2_mode running_mode;

	// Used only for GET or SET modes
	const char *frame_name;
	// Used only for SET mode
	const char *frame_value;
} ID3v2_Config;

/**
 * Initializes the config using the arguments passed.
 */
void initialize_global_config(int argc, char *argv[]);

/**
 * Get a pointer to the global config *after* it has been initialized.
 * @return the same pointer to the global config every time
 */
ID3v2_Config *get_global_config();

#endif //ID3V2_CONFIG_H
