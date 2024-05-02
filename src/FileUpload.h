/**
 * FileUpload.h : Function declaration for Uploading file
 */

#ifndef _FILE_UPLOAD_DOT_H
#define _FILE_UPLOAD_DOT_H

#include "file_transfer/Payload.h"

int UploadFile( const char * file_name,  const char * server_name, int port );

#endif
