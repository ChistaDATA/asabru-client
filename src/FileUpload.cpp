/**
 * Client.cpp : Defines the entry point for the client console application.
 * 
 * Build:
 * POSIX   => g++ -o wire.exe wire.cpp Payload.cpp
 * WINDOWS => cl /DWINDOWS_OS wire.cpp ws2_32.lib
 * 
 * Run:
 * POSIX   => ./Client.exe <file-name> <hostname> <port>
 * WINDOWS => Client <file-name> <hostname> <port>
 * 
 * Eg. ./Client.exe sample.txt 127.0.0.1 8000
*/


#include "FileUpload.h"
#include "CClientSocket.h"

int UploadFile(const char * file_name, const char * server_name, int port )
{

    if (!StartSocket()) {
      fprintf(stdout, "Failed To Initialize Socket Library\n");
      return -1;
    }

    int size_file = ComputeFileSize(file_name);
    printf("File name: %s\tFile size: %d\n", file_name, size_file);

    // Initialize client socket class
    CClientSocket c_sock(server_name, port);

    /* Socket connection has been created. Initiate protocol communation */

    /* Create Handshake packet and Send to socket */
    T_HAND_SHAKE send_packet = MakeHandShake();

    c_sock.SendBytes( &send_packet, sizeof(send_packet));

    /*  Wait for Acknowledgment packet ( sleep 5s )*/
    #ifdef WINDOWS_OS
        Sleep(5);
    #else
        sleep(5);
    #endif

    /* Receive data from socket ( acknowledgment ) */
    char buffer[32];
    int num_read = 32;

    if ( !c_sock.Receive(buffer, &num_read) ) {
        std::cout << "Could not Receive Packet " << std::endl;
        return 1;
    }

    /* Copy the char buffer received from socket into a T_ACK struct */
    T_ACK ack;
    memcpy(&ack, buffer, sizeof(T_ACK));

    // Verify if it is acknowledgment packet
    if ( ack.packet_type != 2 ) {
        std::cout << "Acknowledged Packet is not correct...........\n" << std::endl;
        return 1;
    }

    // Received acknowledment and ready to transfer file
    std::cout <<std::endl << "Ready to Transfer File ...................\n" << std::endl;

    // Create file metadata struct and send to socket
    T_FILE_META file_meta_data = MakeFileMeta(file_name, size_file);
    c_sock.SendBytes((char*) &file_meta_data,sizeof(file_meta_data));

    // Receive acknowledgment from server
    if ( !c_sock.RecvBlocking(buffer, num_read) ) {
        std::cout << "Could not Receive Packet " << std::endl;
        return 1;
    }
        
    memcpy(&ack, buffer, sizeof(T_ACK));
    if ( ack.packet_type != 2 ) {
        std::cout << "Acknowledged Packet is not correct......" << std::endl;
        return 1;
    }
    

    /* Send file in chunks of size */
    std::cout << "Now, we will iterate to send the content of the file......" << std::endl;

    // This stores the number of bytes read from the file
    int num = 0;

    // We will be sending chunk sizes of 5000
    int chunk_size = sizeof(T_FILE_CHUNK) + 5000;
    int packet_sequence = 1;
   
    // open file in read binary (rb) mode
    FILE * fp = fopen(file_name, "rb");

    if  (fp == nullptr) {
        std::cout << "Failed to Open File = " << file_name << std::endl;
        return 1;
    }

    
    std::cout << "Opened File.............." << file_name << std::endl;
    char  read_buffer[4096];

    /**
     * Here we are reading from the file in byte sizes of 4096. From each read, we are creating 
     * the file chunk packet to be send. Next we send the packet through the socket connection. 
    */
    while ( (num = fread(read_buffer, 1, 4096, fp)) == 4096 ) {

        std::cout << "Sequence      ............... " << packet_sequence << std::endl;
        std::cout << "Sending Bytes ............... " << num << std::endl;

        // Create file chunk packet
        T_FILE_CHUNK * chunk = MakeBufferPack(read_buffer, 4096, packet_sequence++);

        // Send file chunk packet
        c_sock.SendBytes((char*) chunk, chunk_size);

        // free up the chunk memory
        free(chunk);
    }
    std::cout << "Residue = " << num << std::endl; // Remaining bytes that was read when it came out of the above loop
    std::cout << "Coming out of the Send Loop " << "Send Residue" << std::endl;

    /**
     * If there are any residue bytes remaining, 
     * we will send it as a file chunk
    */
    if ( num > 0 ) {
        T_FILE_CHUNK * chunk = MakeBufferPack( read_buffer, num, packet_sequence++ );
        c_sock.SendBytes((char*)chunk, chunk_size);
    }
    
    // Close file pointer as we are done reading from the file.
    fclose(fp);
    std::cout << "Finished Sending File, About to send EOF " << std::endl;

    // Send EOF packet, so the server gets confirmation that all packets have been sent.
    T_FILE_EOF eof = MakeEof();
    c_sock.SendBytes((char*)&eof,sizeof(eof));
    std::cout << "Finished Sending EOF " << std::endl ;

    // Close socket connection and clean up.
    c_sock.Close(); 
    Cleanup();

    return 0;
}
