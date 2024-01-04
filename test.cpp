#include <iostream>
#include <sstream>

// Function to simplify PRIVMSG command to channel and message
std::string simplifyPrivMsg(const std::string& fullPrivMsg) {
    std::istringstream iss(fullPrivMsg);
    std::string command, channel, message;
    iss >> command >> channel; // Read the command and channel
    std::getline(iss, message); // Read the rest as the message

    // Trim leading and trailing whitespaces from message
    message.erase(0, message.find_first_not_of(" \t\n\r\f\v"));
    message.erase(message.find_last_not_of(" \t\n\r\f\v") + 1);

    if (command == "PRIVMSG" && channel.substr(0, 1) == "#") {
        return channel + " " + message;
    }

    return ""; // Invalid format or not a PRIVMSG command targeting a channel
}

int main() {
    std::string fullPrivMsg = "PRIVMSG #exempla :Your message goes here";
    
    std::string simplifiedMsg = simplifyPrivMsg(fullPrivMsg);
    if (!simplifiedMsg.empty()) {
        std::cout << "Simplified message: " << simplifiedMsg << std::endl;
    } else {
        std::cout << "Invalid or not a channel PRIVMSG command." << std::endl;
    }

    return 0;
}


