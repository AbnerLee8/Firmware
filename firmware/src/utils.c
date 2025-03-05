/******************************************************************************
  @File Name
    utils.c

  @Summary
    Utility Functions

  @Description

    
*******************************************************************************/
#include "utils.h"

// .............................................................................

// *****************************************************************************
// *****************************************************************************
// STRINGS to VALUE
// *****************************************************************************
// *****************************************************************************

// ................................................................................
// ................................................................................
// Convert given string to a UNSIGNED 32-bit value
// Return err: 0 if ok, < 0 if error
// ................................................................................
int utils_str_to_uint32(char *msg, uint32_t *u32_val) 
{
    int err;
    char *endptr;
    unsigned long u_val = 0;

    // Assume success
    err = 0;

    // Convert to unsigned long, base 10.
    // endptr -s set to the first invalid character encountered during the conversion
    u_val = strtoul(msg, &endptr, 10);

    // Check for errors

    if (endptr == msg || *endptr != '\0') {
        // If endptr is equal to the original string str, it means no conversion was
        // performed indicating an invalid input.
        // If *endptr is not the null terminator ('\0'), it means there are trailing
        // characters after the valid number, indicating an invalid input.
        err = -1;
    } else if (u_val > UINT32_MAX) {
        // If ul_val is greater than UINT32_MAX, it means the value exceeds the range 
        // representable by uint32_t.
        err = -2;
    } else {
        // Conversion success:
        *u32_val = (uint32_t)u_val;;
    }

    return (err);   
}
// ................................................................................
// Convert given string to a SIGNED 32-bit value
// Return err: 0 if ok, < 0 if error
// ................................................................................
int utils_str_to_int32(char *msg, int32_t *i32_val) 
{
    int err;
    char *endptr;
    long l_val = 0;

    // Assume success
    err = 0;

    // Convert to long, base 10.
    // endptr is set to the first invalid character encountered during the conversion
    l_val = strtol(msg, &endptr, 10);

    // Check for errors
    if (endptr == msg || *endptr != '\0') {
        // If endptr is equal to the original string msg, it means no conversion was
        // performed indicating an invalid input.
        // If *endptr is not the null terminator ('\0'), it means there are trailing
        // characters after the valid number, indicating an invalid input.
        err = -1;
    } else if (l_val < INT32_MIN || l_val > INT32_MAX) {
        // If l_val is outside the range of int32_t, it means the value exceeds the range 
        // representable by int32_t.
        err = -2;
    } else {
        // Conversion success:
        *i32_val = (int32_t)l_val;
    }

    return (err);   
}
// ................................................................................
// Convert given string to a UNSIGNED 16-bit value
// Return err: 0 if ok, < 0 if error
// ................................................................................
int utils_str_to_uint16(char *msg, uint16_t *u16_val) 
{
    int err;
    uint32_t u32_val = 0;

    // Assume success
    err = 0;

    err = utils_str_to_uint32(msg, &u32_val);

    if (err == 0)
    {
        if (u32_val < UINT16_MAX)
        {
            *u16_val = (uint16_t)u32_val;
        } else {
           err = -3;
        }
    }

    return (err);   
}
// ................................................................................
// Convert given string to a SIGNED 8-bit value
// Return err: 0 if ok, < 0 if error
// ................................................................................
int utils_str_to_int8(char *msg, int8_t *i8_val) 
{
    int err;
    int32_t i32_val = 0;

    // Assume success
    err = 0;

    err = utils_str_to_int32(msg, &i32_val);

    if (err == 0)
    {
        if (i32_val >= INT8_MIN && i32_val <= INT8_MAX)
        {
            *i8_val = (int8_t)i32_val;
        } else {
            err = -3;
        }
    }

    return (err);   
}
// ................................................................................
// Convert given string to a UNSIGNED 8-bit value
// Return err: 0 if ok, < 0 if error
// ................................................................................
int utils_str_to_uint8(char *msg, uint8_t *u8_val) 
{
    int err;
    uint32_t u32_val = 0;

    // Assume success
    err = 0;

    err = utils_str_to_uint32(msg, &u32_val);

    if (err == 0)
    {
        if (u32_val < UINT8_MAX)
        {
            *u8_val = (uint8_t)u32_val;
        } else {
           err = -3;
        }
    }

    return (err);   
}

// ................................................................................
// Convert given string to a BOOLEAN value
// String must be "true" or "false"
// Return err: 0 if ok, < 0 if error
// ................................................................................
int utils_str_to_bool(char *msg, bool *b_val) 
{
    int err;
    bool b_result = false;

    // Assume success
    err = 0;

    // First verify string converts to boolean
    if (strcmp(msg, "true") == 0)
    {
        b_result = true;
    } else if (strcmp(msg, "false") == 0) 
    {
        b_result = false;
    } else {
        err = -1;
    }
    // Update data
    *b_val = b_result;

    return (err);   
}
// ................................................................................
// Convert given string to a BOOLEAN value
// String must be "on" or "off"
// Return err: 0 if ok, < 0 if error
// ................................................................................
int utils_on_off_str_to_bool(char *msg, bool *b_val) 
{
    int err;
    bool b_result = false;

    // Assume success
    err = 0;

    // First verify string converts to boolean
    if (strcmp(msg, "on") == 0)
    {
        b_result = true;
    } else if (strcmp(msg, "off") == 0) 
    {
        b_result = false;
    } else {
        err = -1;
    }
    // Update data
    *b_val = b_result;

    return (err);   
}

// *****************************************************************************
// *****************************************************************************
// VALUE to STRING
// *****************************************************************************
// *****************************************************************************

// ................................................................................
// Convert given UNSIGNED 32-bit value to string
// Return err: 0 if ok, < 0 if error
// ................................................................................
int utils_uint32_to_str(char *msg, uint32_t u32_val) 
{
    if (msg == NULL)
    {
        return (-1);
    }

    // Convert num to string
    sprintf(msg, "%lu", u32_val);

    return (0);
}
// ................................................................................
// Convert given UNSIGNED 16-bit value to string
// Return err: 0 if ok, < 0 if error
// ................................................................................
int utils_uint16_to_str(char *msg, uint16_t u16_val) 
{
    if (msg == NULL)
    {
        return (-1);
    }

    // Convert num to string
    sprintf(msg, "%hu", u16_val);

    return (0);
}
// ................................................................................
// Convert given SIGNED 8-bit value to string
// Return err: 0 if ok, < 0 if error
// ................................................................................
int utils_int8_to_str(char *msg, int8_t i8_val) 
{
    if (msg == NULL)
    {
        return (-1);
    }

    // Convert num to string
    sprintf(msg, "%d", i8_val);

    return (0);
}
// ................................................................................
// Convert given UNSIGNED 8-bit value to string
// Return err: 0 if ok, < 0 if error
// ................................................................................
int utils_uint8_to_str(char *msg, uint8_t u8_val) 
{
    if (msg == NULL)
    {
        return (-1);
    }

    // Convert num to string
    sprintf(msg, "%u", u8_val);

    return (0);
}
// ................................................................................
// Convert given BOOLEAN value to string
// Return err: 0 if ok, < 0 if error
// ................................................................................
int utils_bool_to_str(char *msg, bool b_val) 
{
    if (msg == NULL)
    {
        return (-1);
    }

    // Create required string
    if (b_val)
    {
        sprintf(msg, "%s", "true");
    } else {
        sprintf(msg, "%s", "false");
    }
    return (0);
}
// ................................................................................
// Convert given On/OFF boolean value to string
// Return err: 0 if ok, < 0 if error
// ................................................................................
int utils_bool_to_on_off_str(char *msg, bool b_val) 
{
    if (msg == NULL)
    {
        return (-1);
    }

    // Create required string
    if (b_val)
    {
        sprintf(msg, "%s", "on");
    } else {
        sprintf(msg, "%s", "off");
    }
    return (0);
}

// .............................................................................
