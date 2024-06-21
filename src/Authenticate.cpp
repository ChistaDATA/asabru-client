////
//// Created by JOSEPH ABRAHAM on 07/04/24.
////

#include "Authenticate.h"

bool AuthenticateUser(const std::string username, const std::string password, const std::string host, const int port) {
	// Resolve the Handler class
	CommandDispatcher::RegisterCommand<BaseComputationCommand>("HTTPRequestCommand");
	ComputationContext context;

	// Set up request parameters
	context.Put("host", host);
	context.Put("port", port);
	context.Put("method", simple_http_server::HttpMethod::POST);
	context.Put("requestUri", std::string("/auth/login"));
	context.Put("content-type", std::string("application/json"));

	// Construct JSON payload
	std::string jsonContent = "{\"username\":\"" + username + "\",\"password\":\"" + password + "\"}";
	context.Put("content", jsonContent);

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

	// Parse response content to extract token
	std::string token;
	try {
		nlohmann::json body = nlohmann::json::parse(response_content);
		token = body["token"].get<std::string>();
	} catch (const std::exception &e) {
		LOG_ERROR("Error parsing response JSON: " + std::string(e.what()));
		return false;
	}

	return true;
}