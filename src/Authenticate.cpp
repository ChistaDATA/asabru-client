////
//// Created by JOSEPH ABRAHAM on 07/04/24.
////

#include "Authenticate.h"

bool AuthenticateUser(const std::string username, const std::string password,
                      const std::string host, const int port) {
    // Resolve the Handler class
    CommandDispatcher::RegisterCommand<BaseComputationCommand>(
            "HTTPRequestCommand");
    ComputationContext context;

    // Set up request parameters
    context.Put("host", host);
    context.Put("port", port);
    context.Put("method", simple_http_server::HttpMethod::POST);
    context.Put("requestUri", std::string("/auth/login"));

    // Construct JSON payload
    std::string jsonContent =
            "{\"username\":\"" + username + "\",\"password\":\"" + password + "\"}";
    context.Put("content", jsonContent);
    context.Put("request", std::string("TestRequest"));

    // Dispatch HTTP request command
    if (!CommandDispatcher::Dispatch("HTTPRequestCommand", &context)) {
        LOG_ERROR("Failed to dispatch HTTP request command.");
        return false;
    }

    // Retrieve and validate response
    std::string response;
    if (!RetrieveStringFromContext(context, "response", response)) {
        return false;
    }

    // Retrieve and validate response content
    std::string response_content;
    if (!RetrieveStringFromContext(context, "response_content",
                                   response_content)) {
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

// Helper function to retrieve string value from context and validate it
bool RetrieveStringFromContext(ComputationContext &context,
                               const std::string &key, std::string &value) {
    std::any value_any = context.Get(key);
    if (value_any.type() != typeid(std::string)) {
        LOG_ERROR("Value for key \"" + key + "\" not found or not a string.");
        return false;
    }

    value = std::any_cast<std::string>(value_any);
    if (value.empty()) {
        LOG_ERROR("Value for key \"" + key + "\" is empty.");
        return false;
    }

    return true;
}