#include <pgmspace.h>
#define AWSCONFIG

/**
 * size of the message that will be sended
 */
#define DOCUMENT_SIZE 256

/**
 * your wifi credentials
 * preferably it should be a 2.4ghz network
 *
 */
const char WIFI_SSID[] = "YOUR-SSID";
const char WIFI_PASSWORD[] = "YOUR-WIFI-PASSWORD";

/**
 * ntp server
 * consult https://www.ntppool.org for
 */
const char *ntpServer = "mx.pool.ntp.org";

/**
 * device data endpoint
 * you will find it in
 * aws console -> iotcore -> settings -> Device data endpoint
 */
const char AWS_IOT_ENDPOINT[] = "YOUR-AWS-ENDPOINT";

/**
 * Create a new thing in aws console
 * before it is created you must download all the certificates
 * replace your certificates bellow
 * AmazonRootCA1.pem
 * ...-certificate.pem
 * ...-private.pem.pem.key
 */
#define THINGNAME "YOUR-THING-NAME"

// AmazonRootCA1.pem
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
...
-----END CERTIFICATE-----
)EOF";

// certificate.pem
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
...
-----END CERTIFICATE-----
)KEY";

// private.pem.pem.key
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
...
-----END RSA PRIVATE KEY-----
)KEY";