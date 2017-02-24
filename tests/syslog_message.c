#include "test.h"

void test_syslog_message__returns_false_on_garbage(void) {
	syslog_message_t msg = {};

	cl_assert_(false == parse_syslog_message_t(NULL, &msg), "NULL should not parse");
	cl_assert_(false == parse_syslog_message_t("", &msg), "Empty string should not parse");
	cl_assert_(false == parse_syslog_message_t("stuff", &msg), "'stuff' should not parse");
	cl_assert_(false == parse_syslog_message_t("<abc>stuff", &msg), "'<abc>stuff' should not parse");
	cl_assert_(false == parse_syslog_message_t("<abc>1 stuff", &msg), "'<abc>1 stuff' should not parse");
	cl_assert_(false == parse_syslog_message_t("<stuff", &msg), "'<stuff' should not parse");
	cl_assert_(false == parse_syslog_message_t("\n", &msg), "New line should not parse");
	cl_assert_(false == parse_syslog_message_t("\t\t\t\t\t\t\t\t\r\n", &msg), "Tabs should not parse");

}

void test_syslog_message__can_be_parsed(void) {
	syslog_message_t msg = {};

	char * mm = "<165>1 2016-12-16T12:00:00.000Z hostname appname PROCID MSGID Logging message...";

	if (!parse_syslog_message_t(mm, &msg)) {
		cl_fail("Could not parse the syslog message");
	}

	cl_assert_equal_i(msg.severity, 5);
	cl_assert_equal_i(msg.facility, 20);
	cl_assert_equal_i(msg.pri_value, 165);
	cl_assert_equal_s(msg.syslog_version, "1");

	cl_assert_equal_s(msg.message_id, "MSGID");
	cl_assert_equal_s(msg.hostname, "hostname");
	cl_assert_equal_s(msg.appname, "appname");
	cl_assert_equal_s(msg.process_id, "PROCID");

	cl_assert_equal_s(msg.message, "Logging message...");

	char timestring[100];
	strftime(timestring, 100, "%x - %I:%M%p", &msg.timestamp);

	cl_assert_equal_s(timestring, "12/16/16 - 12:00PM");

	cl_assert_(msg.structured_data == 0, "There should be no structured data");

	free_syslog_message_t(&msg);
}

void test_syslog_message__can_fail(void) {
	syslog_message_t msg = {};

	char * mm = "s<165>1 2016-12-16T12:00:00.000Z hostname appname PROCID MSGID Logging message...";

	if (parse_syslog_message_t(mm, &msg)) {
		cl_fail("Should not be able to parse this syslog message");
	}

}

void test_syslog_message__can_be_parsed_with_a_different_message(void) {
	syslog_message_t msg = {};

	char * mm = "<165>1 2016-12-16T12:00:00.000Z adifferenthostname adifferentappname ID MSGID2 Logging message... but this one is kinda different";

	if (!parse_syslog_message_t(mm, &msg)) {
		cl_fail("Could not parse the syslog message");
	}

	cl_assert_equal_i(msg.severity, 5);
	cl_assert_equal_i(msg.facility, 20);
	cl_assert_equal_i(msg.pri_value, 165);
	cl_assert_equal_s(msg.syslog_version, "1");

	cl_assert_equal_s(msg.message_id, "MSGID2");
	cl_assert_equal_s(msg.hostname, "adifferenthostname");
	cl_assert_equal_s(msg.appname, "adifferentappname");
	cl_assert_equal_s(msg.process_id, "ID");

	cl_assert_equal_s(msg.message, "Logging message... but this one is kinda different");

	char timestring[100];
	strftime(timestring, 100, "%x - %I:%M%p", &msg.timestamp);

	cl_assert_equal_s(timestring, "12/16/16 - 12:00PM");

	cl_assert_(msg.structured_data == 0, "There should be no structured data");

	free_syslog_message_t(&msg);
}
