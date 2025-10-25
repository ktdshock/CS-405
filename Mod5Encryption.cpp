// Encryption.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <ctime>

/// <summary>
/// encrypt or decrypt a source string using the provided key
/// </summary>
/// <param name="source">input string to process</param>
/// <param name="key">key to use in encryption / decryption</param>
/// <returns>transformed string</returns>
std::string encrypt_decrypt(const std::string& source, const std::string& key)
{
    const auto key_length = key.length();
    const auto source_length = source.length();

    assert(key_length > 0);
    assert(source_length > 0);

    std::string output = source;

    // loop through the source string char by char
    for (size_t i = 0; i < source_length; ++i)
    {
        // XOR each character with the key (wrap around key with modulo)
        output[i] = source[i] ^ key[i % key_length];
    }

    assert(output.length() == source_length);

    return output;
}

/// <summary>
/// load a text file into a string
/// </summary>
std::string read_file(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        throw std::runtime_error("Could not open input file: " + filename);
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();  // read entire file contents into buffer
    return buffer.str();
}

/// <summary>
/// get the student name (first line of the file)
/// </summary>
std::string get_student_name(const std::string& string_data)
{
    std::string student_name;
    size_t pos = string_data.find('\n');
    if (pos != std::string::npos)
    {
        student_name = string_data.substr(0, pos);
    }
    return student_name;
}

/// <summary>
/// save string data to a text file with student name, date, key, and data
/// </summary>
void save_data_file(const std::string& filename, const std::string& student_name, const std::string& key, const std::string& data)
{
    std::ofstream file(filename);
    if (!file)
    {
        throw std::runtime_error("Could not open output file: " + filename);
    }

    // Get current date (yyyy-mm-dd)
    time_t now = time(nullptr);
    tm* ltm = localtime(&now);
    char date[11];
    strftime(date, sizeof(date), "%Y-%m-%d", ltm);

    file << student_name << "\n";
    file << date << "\n";
    file << key << "\n";
    file << data << "\n";
}

int main()
{
    std::cout << "Encryption Decryption Test!" << std::endl;

    const std::string file_name = "inputdatafile.txt";
    const std::string encrypted_file_name = "encrypteddatafile.txt";
    const std::string decrypted_file_name = "decrytpteddatafile.txt";
    const std::string key = "password";

    // read original input file
    const std::string source_string = read_file(file_name);

    // get the student name from the data file
    const std::string student_name = get_student_name(source_string);

    // encrypt source string with key
    const std::string encrypted_string = encrypt_decrypt(source_string, key);

    // save encrypted string to file
    save_data_file(encrypted_file_name, student_name, key, encrypted_string);

    // decrypt encrypted string with key
    const std::string decrypted_string = encrypt_decrypt(encrypted_string, key);

    // save decrypted string to file
    save_data_file(decrypted_file_name, student_name, key, decrypted_string);

    std::cout << "Read File: " << file_name
              << " - Encrypted To: " << encrypted_file_name
              << " - Decrypted To: " << decrypted_file_name << std::endl;

    return 0;
}