/*******************************************************************************
  File Name:
    app_mem.c

  Summary:
    Serial flash memory task.

  Description:
 *******************************************************************************/
#include "app_mem.h"

// .............................................................................

static uint8_t CACHE_ALIGN fswork[512];

const char flash_dev_name[] = "/dev/mtda1";
const char flash_mount_name[] = "/mnt/mem1";

#define MAX_DATA_WRITE_LEN 512

// .............................................................................
void debug_dump_output(uint8_t *buf, int len)
{
#if 1    
    DBGprintf("dump:\n");
    for (int i = 0; i < len; i++) {
        DBGprintf("%02X ", buf[i] & 0xFF);
        if ((i & 0x0F) == 0x0F) {
            DBGprintf("\n");
        }
    }
    DBGprintf("\n");
#endif
}

// .............................................................................
// Read the filenames of all the files in the specified directory
// .............................................................................
void app_mem_read_dir() 
{
    SYS_FS_FSTAT fileStatus;
    SYS_FS_RESULT res;

    // Open the directory
    SYS_FS_HANDLE dirHandle = SYS_FS_DirOpen("/mnt/mem1");
    
    if (dirHandle == SYS_FS_HANDLE_INVALID) {
        return;
    }
        
    res = SYS_FS_DirRewind(dirHandle);
    if (res != SYS_FS_RES_SUCCESS) {
        return;
    }
    
    // !gb! Added this line so that could call this function twice!
    // !gb! Without this line, 2nd call returned invalid handle... don;t know why
    memset(&fileStatus, 0, sizeof(SYS_FS_FSTAT));       // !dbg!

    if (dirHandle != SYS_FS_HANDLE_INVALID) {

        // Read each file in the directory
        while (SYS_FS_DirRead(dirHandle, &fileStatus) == SYS_FS_RES_SUCCESS) {
            if (fileStatus.fname[0] == '\0') break;  // End of directory
            printf("File: %s\n", fileStatus.fname);   // Display or process file names
            //dbg_file_cntr++;        // !dbg!
        }

        // Close the directory
        SYS_FS_DirClose(dirHandle);
    } else {
        // Handle error in opening directory
    }

}

// .............................................................................
// See if a particular file exists
// e.g. filePath = "/mnt/mem1/text1.txt"
// .............................................................................
bool app_mem_file_exists(const char *filePath) {
    SYS_FS_FSTAT fileStatus;
    
    memset(&fileStatus, 0, sizeof(SYS_FS_FSTAT));
    
    // Check the file status
    if (SYS_FS_FileStat(filePath, &fileStatus) == SYS_FS_RES_SUCCESS) {
        // File exists
        return true;
    } else {
        // File does not exist
        return false;
    }
}
// .............................................................................
// Write to a file (overwrite if already exists)
// .............................................................................
void app_mem_file_write_ch(const char *filePath, char *data_to_w) 
{
    int c;
    int len;
    SYS_FS_HANDLE fileHandle;
    
    len = strlen(data_to_w);

    strcpy((char *)fswork, data_to_w);
    fileHandle = SYS_FS_FileOpen(filePath, (SYS_FS_FILE_OPEN_WRITE));
    c = SYS_FS_FileWrite(fileHandle, (const void *)fswork, len);
    SYS_FS_FileClose(fileHandle);
}
// .............................................................................
// Append to a file (create if it doesn't exists)
// .............................................................................
int app_mem_file_app_ch(char *fileName, char *data_to_w) 
{
    int res;
    int len;
    SYS_FS_HANDLE fileHandle;
    char fullPath[100];
    size_t required_path_length;
    
    // FIRST - verify can go ahead with the operation
    
    // Check if fileName is NULL or empty
    if (fileName == NULL || fileName[0] == '\0') {
        // Filename is null or empty
        return -1;
    }
    // Check if data_to_w is NULL
    if (data_to_w == NULL) {
        // data_to_w is NULL
        return -2;
    }
    // Calculate the required path length and check for overflow
    required_path_length = strlen(flash_mount_name) + 1 + strlen(fileName) + 1;
    if (required_path_length > sizeof(fullPath)) {
        // Full path exceeds buffer size
        return -3;
    }    
    
    // SECOND: Operate!
    
    // Combine mount name and file name into full_path
    snprintf(fullPath, sizeof(fullPath), "%s/%s", flash_mount_name, fileName);    
    
    len = strlen(data_to_w);
    
    strcpy((char *)fswork, data_to_w);
//    fileHandle = SYS_FS_FileOpen(filePath, (SYS_FS_FILE_OPEN_APPEND));
    fileHandle = SYS_FS_FileOpen(fullPath, (SYS_FS_FILE_OPEN_APPEND));
    if (fileHandle == SYS_FS_HANDLE_INVALID) {
        // Invalid handle
        return -4;
    }
    res = SYS_FS_FileWrite(fileHandle, (const void *)fswork, len);
    if (res == -1) {
        // Failed to write data to file
    }
    res = SYS_FS_FileClose(fileHandle);
    if (res != SYS_FS_RES_SUCCESS) {
        // Failed to close file
        return -6;
    }
    return 0;
}
// .............................................................................
// Write to a file (overwrite if already exists)
// .............................................................................
int app_mem_file_write(char *fileName, const uint8_t *data_to_w, size_t data_len) 
{
    int res;
    int len;
    SYS_FS_HANDLE fileHandle;
    char fullPath[100];
    size_t required_path_length;
    
    // FIRST - verify can go ahead with the operation
    
    // Check if fileName is NULL or empty
    if (fileName == NULL || fileName[0] == '\0') {
        // Filename is null or empty
        return -1;
    }

    if (data_len > MAX_DATA_WRITE_LEN) {
        // Invalid data length
        return -3;
    }
    // Calculate the required path length and check for overflow
    required_path_length = strlen(flash_mount_name) + 1 + strlen(fileName) + 1;
    if (required_path_length > sizeof(fullPath)) {
        // Full path exceeds buffer size
        return -4;
    }    
    
    // SECOND: Operate!
    
    // Combine mount name and file name into full_path
    snprintf(fullPath, sizeof(fullPath), "%s/%s", flash_mount_name, fileName);    
    
    len = strlen(data_to_w);
    
//    strcpy((char *)fswork, data_to_w);
    fileHandle = SYS_FS_FileOpen(fullPath, (SYS_FS_FILE_OPEN_WRITE));
    if (fileHandle == SYS_FS_HANDLE_INVALID) {
        // Invalid handle
        return -5;
    }
    res = SYS_FS_FileWrite(fileHandle, data_to_w, data_len);
//    res = SYS_FS_FileWrite(fileHandle, (const void *)fswork, len);
    if (res == -1) {
        // Failed to write data to file
    }
    res = SYS_FS_FileClose(fileHandle);
    if (res != SYS_FS_RES_SUCCESS) {
        // Failed to close file
        return -7;
    }
    return 0;
}
// .............................................................................
// Append to a file (create if it doesn't exists)
// .............................................................................
int app_mem_file_app(char *fileName, const uint8_t *data_to_w, size_t data_len) 
{
    int res;
    int len;
    SYS_FS_HANDLE fileHandle;
    char fullPath[100];
    size_t required_path_length;
    
    // FIRST - verify can go ahead with the operation
    
    // Check if fileName is NULL or empty
    if (fileName == NULL || fileName[0] == '\0') {
        // Filename is null or empty
        return -1;
    }
    
    //if ((data_len == 0) || (data_len > MAX_DATA_WRITE_LEN)) {
    if ((data_len == 0) || (data_len > 1024)) {
        // Invalid data length
        return -3;
    }
    // Calculate the required path length and check for overflow
    required_path_length = strlen(flash_mount_name) + 1 + strlen(fileName) + 1;
    if (required_path_length > sizeof(fullPath)) {
        // Full path exceeds buffer size
        return -4;
    }    
    
    // SECOND: Operate!
    
    // Combine mount name and file name into full_path
    snprintf(fullPath, sizeof(fullPath), "%s/%s", flash_mount_name, fileName);    
    
    len = strlen(data_to_w);
    
    fileHandle = SYS_FS_FileOpen(fullPath, (SYS_FS_FILE_OPEN_APPEND));
    if (fileHandle == SYS_FS_HANDLE_INVALID) {
        // Invalid handle
        return -5;
    }
    res = SYS_FS_FileWrite(fileHandle, data_to_w, data_len);
    if (res == -1) {
        // Failed to write data to file
    }
    res = SYS_FS_FileClose(fileHandle);
    if (res != SYS_FS_RES_SUCCESS) {
        // Failed to close file
        return -7;
    }
    return 0;
}
// .............................................................................
// Read contents of a file
// .............................................................................
void app_mem_file_read_ch(const char *filePath, char *data_to_r) 
{
    int c;
    int fileSize;
    SYS_FS_HANDLE fileHandle;

    fileHandle = SYS_FS_FileOpen(filePath, (SYS_FS_FILE_OPEN_READ));
    fileSize = SYS_FS_FileSize(fileHandle);

    if (fileSize > 512) fileSize = 512;
    c = SYS_FS_FileRead(fileHandle, (void *)data_to_r, fileSize);
    SYS_FS_FileClose(fileHandle);

}
// .............................................................................
// Read contents of a file
// .............................................................................
int app_mem_file_read(const char *fileName, uint8_t *data_to_r, size_t data_size) 
{
    int c;
    int fileSize;
    SYS_FS_HANDLE fileHandle;
    int res;

    char fullPath[100];
    
    // Combine mount name and file name into full_path
    snprintf(fullPath, sizeof(fullPath), "%s/%s", flash_mount_name, fileName);  
    
    

    fileHandle = SYS_FS_FileOpen(fullPath, (SYS_FS_FILE_OPEN_READ));
    if (fileHandle == SYS_FS_HANDLE_INVALID) {
        // Failed to open file
        return -1;
    }
    fileSize = SYS_FS_FileSize(fileHandle);
    //dbg_fileSize = fileSize;    // !dbg!
    if (fileSize > 512) fileSize = 512;

    c = SYS_FS_FileRead(fileHandle, data_to_r, data_size);
    
    
    if (c == -1) {
        // Failed to read data from file
    } else {
        // All okay
    }

    res = SYS_FS_FileClose(fileHandle);
    if (res != SYS_FS_RES_SUCCESS) {
        // Failed to close file
        c = -3;
    }
    return c;
}
// .............................................................................
// Delete a file (if it exists)
// .............................................................................
int app_mem_file_del(const char *fileName) 
{
    int res;
    
    res = SYS_FS_FileDirectoryRemove(fileName);
    
    return res;
}

// .............................................................................
// Empty a file
// .............................................................................
int app_mem_file_empty(char *fileName) 
{
    int res;
    uint8_t dummy;
    
    res = app_mem_file_write(fileName, &dummy, 0) ;
    
    return res;
}

// .............................................................................
// Get file size
// .............................................................................
int app_mem_file_size(char *fileName) 
{
    int fileSize;
    SYS_FS_HANDLE fileHandle;
    
    fileSize = -1;
    
    fileHandle = SYS_FS_FileOpen(fileName, (SYS_FS_FILE_OPEN_READ));
    if (fileHandle == SYS_FS_HANDLE_INVALID) {
        // Invalid handle
        return -2;
    }    
    fileSize = SYS_FS_FileSize(fileHandle);
    SYS_FS_FileClose(fileHandle);

    return (fileSize);
}
// .............................................................................
// Rename a file
// .............................................................................
int app_mem_file_rename(char *fileNameOld, char *fileNameNew) 
{
    int res;
    
    res = SYS_FS_FileDirectoryRenameMove(fileNameOld, fileNameNew);
    
    // Attempt to rename the file
    if (res == SYS_FS_RES_SUCCESS)
    {
        res = 0;
    }
    else
    {
        res = -1;
    }    
    return res;
}
// .............................................................................

// .............................................................................
// Task initializations.
// .............................................................................
void APP_MEM_Initialize ( void )
{
}

// .............................................................................
// Memory task
// .............................................................................
void APP_MEM_Tasks ( void )
{
    SYS_FS_HANDLE fileHandle;
    // Task waits Power Task has powered everything up
    while (!GlobalPowerOn) {
        osDelayMs(100);
    }
    
    // The serial flash memory is on the 1V8 power domain of the MCU

    // Mount the flash device
    while (SYS_FS_Mount(flash_dev_name, flash_mount_name, FAT, 0, NULL) != SYS_FS_RES_SUCCESS) {
        OS_SLEEP(5); //osDelayMs(5);
    }
    if (SYS_FS_Error() == SYS_FS_ERROR_NO_FILESYSTEM)
    {
        // Format required.
        DBGprintf("/mnt/mem1 Format required\n");
        if (SYS_FS_DriveFormat("/mnt/mem1", NULL, (void *)fswork, 512) == SYS_FS_RES_FAILURE) {
            DBGprintf("/mnt/mem1 format failure!\n");
        }
        else {
            bool f;
            DBGprintf("/mnt/mem1 Format OK!\n");
            f = DRV_MX25L_Read(0, (void *)fswork, 512, 0x0000);
            DBGprintf("read [%d] at 0x0000\n", f);
            debug_dump_output(fswork, 512);
        }
    }
    else {
        // Flash device is mounted
        DBGprintf("/mnt/mem1 ok\n");
    }
    SYS_FS_CurrentDriveSet("/mnt/mem1");
    
    DBGprintf("current drive set = %d\n", SYS_FS_Error());
   // DBGprintf("current file: %s\n", fileName1);


    /*
    // ********************
    // Unused (test) code    
    // ********************
    if (app_mem_file_exists(pathNameImage1)) {
        len = app_mem_file_size(pathNameImage1);
    }
    */

    osDelayMs(2000);

    /*
    // ********************
    // Unused (test) code    
    // ********************
    res = app_mem_file_rename(dbg_oldFileName, dbg_newFileName);
    app_mem_read_dir();


    osDelayMs(2000);
    strcpy((char *)fswork, "Hello file!\n");
    fileHandle = SYS_FS_FileOpen(fileName1, (SYS_FS_FILE_OPEN_APPEND));
    DBGprintf("file handle=%d, file open error=%d\n", fileHandle, SYS_FS_Error());
    c = SYS_FS_FileWrite(fileHandle, (const void *)fswork, 13);
    SYS_FS_FileClose(fileHandle);
    DBGprintf("%s 13 bytes written\n", fileName1);

        osDelayMs(2000);
        strcpy((char *)fswork, "Seeya file!\n");
        fileHandle = SYS_FS_FileOpen(fileName1, (SYS_FS_FILE_OPEN_APPEND));
        DBGprintf("file handle=%d, file open error=%d\n", fileHandle, SYS_FS_Error());
        c = SYS_FS_FileWrite(fileHandle, (const void *)fswork, 13);
        SYS_FS_FileClose(fileHandle);
        DBGprintf("%s 13 bytes written\n", fileName1);


    osDelayMs(2000);
    strcpy((char *)fswork, "xxxxxxxxxxxxx");
    fileHandle = SYS_FS_FileOpen(fileName1, (SYS_FS_FILE_OPEN_READ));
    fileSize = SYS_FS_FileSize(fileHandle);
    if (fileSize > 512) fileSize = 512;
    c = SYS_FS_FileRead(fileHandle, (void *)fswork, fileSize);
    SYS_FS_FileClose(fileHandle);
    DBGprintf("%s %d bytes read\n", fileName1, c);
    DBGprintf("%s", fswork);
    debug_dump_output(fswork, fileSize);
    
    app_mem_read_dir();
     
     */
    

        
    while (1) {
        OS_SLEEP(1000);        

    }
}
// .............................................................................
