//
// Created by JOSEPH ABRAHAM on 30/04/24.
//

#include <iostream>

#include "UploadZipFile.h"
#include "http/http_message.h"
#include "multipart_parser/multipart_parser.h"

bool UploadZipFile() {
	// Resolve the Handler class
	CommandDispatcher::RegisterCommand<BaseComputationCommand>("HTTPRequestCommand");
	ComputationContext context;

    const std::string host = "localhost";
    const int port =6025;

	// Set up request parameters
	context.Put("host", host);
	context.Put("port", port);
	context.Put("method", simple_http_server::HttpMethod::POST);
	context.Put("requestUri", std::string("/plugins"));

	// Use MultipartParser to get the encoded body content and boundary
	web::http::MultipartParser parser;
	parser.AddParameter("name", "profiler2");
	parser.AddParameter("type", "python_script");
	parser.AddFile("plugin", "/Users/josephabraham/ChistaV3/metrics-agent/example/python_script/script.zip");
	std::string boundary = parser.boundary();
    std::cout << "Boundary : " << boundary <<std::endl;
	std::string body = parser.GenBodyContent();
	std::cout << "Body :" << body << std::endl;
	context.Put("content_type", "multipart/form-data; boundary=" + boundary);
	context.Put("content", body);

	// Dispatch HTTP request command
	if (!CommandDispatcher::Dispatch("HTTPRequestCommand", &context)) {
		LOG_ERROR("Failed to dispatch HTTP request command.");
		return false;
	}

	// Retrieve and validate response
	std::string response = context.GetString("response");
	if (response.empty()) {
		return false;
	}

	// Retrieve and validate response
	std::string response_content = context.GetString("response_content");
	if (response_content.empty()) {
		return false;
	}
	std::cout<<"Response content : " << response_content << std::endl;

	return true;
}