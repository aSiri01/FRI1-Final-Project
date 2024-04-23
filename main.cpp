#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Function to perform HTTP POST request using libcurl
std::string http_post_request(const std::string& url, const std::string& data) {
    CURL *curl;
    CURLcode res;
    std::string response;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, [](char *ptr, size_t size, size_t nmemb, std::string *stream){
            (*stream).append(ptr, size * nmemb);
            return size * nmemb;
        });
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    return response;
}

// Function to read API key from .env file
std::string read_api_key() {
    std::ifstream file(".env");
    std::string line;
    std::string api_key;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            if (line.find("OPENAI_API_KEY") != std::string::npos) {
                api_key = line.substr(line.find("=") + 1);
                break;
            }
        }
        file.close();
    }
    return api_key;
}

int main() {
    // Read OpenAI API key from .env file
    std::string api_key = read_api_key();
    if (api_key.empty()) {
        std::cerr << "Error: OpenAI API key not found in .env file" << std::endl;
        return 1;
    }

    // Set OpenAI API endpoint
    std::string endpoint = "https://api.openai.com/v1/completions";

    // Set the prompt for the conversation
    std::string prompt = "User: ";

    // Loop to continue the conversation
    while (true) {
        // Get user input
        std::string user_input;
        std::cout << "You: ";
        std::getline(std::cin, user_input);
        if (user_input.empty()) {
            break;
        }

        // Construct the request data
        json data = {
            {"prompt", prompt + user_input},
            {"max_tokens", 150},
            {"temperature", 0.7}
        };

        // Convert JSON data to string
        std::string data_str = data.dump();

        // Perform HTTP POST request to OpenAI API
        std::string response = http_post_request(endpoint, data_str);

        // Parse and extract ChatGPT response
        json response_json = json::parse(response);
        std::string chatgpt_response = response_json["choices"][0]["text"];

        // Print ChatGPT response
        std::cout << "ChatGPT: " << chatgpt_response << std::endl;
    }

    return 0;
}
