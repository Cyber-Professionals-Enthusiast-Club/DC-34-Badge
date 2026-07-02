/**
 *  Query-SAO allows users to query the status of hot-pluggable SAO componets
 */

// Set I2C bus to use: Wire, Wire1, etc.
#define WIRE Wire

typedef enum {
    QUERY_SAO_NONE = 0,
    QUERY_SAO_MISSILE_LAUNCHER,
    QUERY_SAO_GATLING_GUN,
    QUERY_SAO_LASER_CANNON,
    QUERY_SAO_NUM_TYPES // _NUM_TYPES should be the last value in the sequence
} query_sao_return_t;

typedef struct {
    query_sao_return_t SAO0;
    query_sao_return_t SAO1;
    query_sao_return_t SAO2;
    float battery_percentage;
    unsigned char chipID;
} query_sao_collection_t;

/**
 * query_sao scans the I2C bus and returns a list of devices and their type.
 * @return query_sao_collection_t indicating which slots have something plugged
 * in and what type of device they are. 
 */
query_sao_collection_t query_sao(void);

/**
 * sao_return_to_string takes a query_sao_return_t enum and returns a null 
 * terminated string representing the device.
 * @param returnType enum representing the board ID.
 * @return null terminated string of a human readable string representing 
 */
char * sao_return_to_string(query_sao_return_t returnType);
