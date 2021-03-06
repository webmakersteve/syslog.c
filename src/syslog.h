#ifndef LIB_SYSLOG_H
#define LIB_SYSLOG_H

// Needed for sscanf
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef struct syslog_extended_property_value_t {
  char* key;
  char* value;
} syslog_extended_property_value_t;

typedef struct syslog_extended_property_t {
  char* id;
  syslog_extended_property_value_t * pairs;
  size_t num_pairs;
  char* raw_interned_message;
} syslog_extended_property_t;

typedef struct syslog_message_t {
  const char* message;
  const char* syslog_version;

  int severity;
  int facility;
  int pri_value;

  const char* message_id;

  const char* hostname;
  const char* appname;

  const char* process_id;

  struct tm timestamp;
  enum syslog_type {
    RFC5424=5424
  } syslog_type;

  syslog_extended_property_t * structured_data;
  size_t structured_data_count;

  size_t message_length;

  char* raw_interned_message;
} syslog_message_t;

int parse_syslog_message_t(const char*, syslog_message_t*);
void free_syslog_message_t(syslog_message_t * syslog_message);

#ifdef __cplusplus
}
#endif

#endif
