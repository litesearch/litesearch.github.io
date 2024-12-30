#ifndef LITE_WEB_CRAWLER_HPP
#define LITE_WEB_CRAWLER_HPP

#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <curl/curl.h>

class WebCrawler {
public:
    // Constructor
    WebCrawler() {
        curl_global_init(CURL_GLOBAL_DEFAULT);
    }

    // Destructor
    ~WebCrawler() {
        curl_global_cleanup();
    }

    // Fetch the content of a URL
    std::string fetchWebpage(const std::string& url) {
        CURL* curl = curl_easy_init();
        if (!curl) {
            throw std::runtime_error("Failed to initialize CURL");
        }

        std::string webpageContent;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &webpageContent);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            curl_easy_cleanup(curl);
            throw std::runtime_error("CURL error: " + std::string(curl_easy_strerror(res)));
        }

        curl_easy_cleanup(curl);
        return webpageContent;
    }

    // Extract all links from the webpage content
    std::vector<std::string> extractLinks(const std::string& webpageContent) {
        std::vector<std::string> links;
        std::regex linkRegex(R"(href=["'](http[s]?://[^"']+)["'])");
        std::smatch match;
        std::string::const_iterator searchStart(webpageContent.cbegin());

        while (std::regex_search(searchStart, webpageContent.cend(), match, linkRegex)) {
            links.push_back(match[1].str());
            searchStart = match.suffix().first;
        }

        return links;
    }

    // Check robots.txt for crawl permissions
    bool isCrawlingAllowed(const std::string& url) {
        std::string domain = extractDomain(url);
        std::string robotsUrl = domain + "/robots.txt";

        std::string robotsTxtContent;
        try {
            robotsTxtContent = fetchWebpage(robotsUrl);
        } catch (...) {
            return true; // Assume allowed if robots.txt is not accessible
        }

        return checkRobotsTxt(robotsTxtContent, url.substr(domain.length()));
    }

private:
    // Helper to fetch content from a URL
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* userData) {
        size_t totalSize = size * nmemb;
        userData->append((char*)contents, totalSize);
        return totalSize;
    }

    // Parse robots.txt
    bool checkRobotsTxt(const std::string& robotsTxtContent, const std::string& path) {
        std::istringstream stream(robotsTxtContent);
        std::string line;
        bool userAgentMatch = false;

        while (std::getline(stream, line)) {
            if (line.find("User-agent: *") != std::string::npos) {
                userAgentMatch = true;
            } else if (userAgentMatch && line.find("Disallow:") != std::string::npos) {
                std::string disallowedPath = line.substr(10); // After "Disallow: "
                if (!disallowedPath.empty() && path.find(disallowedPath) == 0) {
                    return false; // Crawling is not allowed
                }
            } else if (line.empty() || line[0] == 'U') {
                userAgentMatch = false;
            }
        }

        return true; // No disallowed paths found
    }

    // Extract domain from URL
    std::string extractDomain(const std::string& url) {
        std::regex domainRegex(R"((http[s]?://[^/]+))");
        std::smatch match;

        if (std::regex_search(url, match, domainRegex)) {
            return match[1].str();
        }

        throw std::runtime_error("Invalid URL: " + url);
    }
};

#endif // WEB_CRAWLER_H
