#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <vector>
//#include <map>

constexpr std::string_view target_directory = "/Users/mac/Downloads/com.kakao.talk";
constexpr std::string_view copy_destination_path = "/Users/mac/Downloads/com.kakao.talkc";
constexpr unsigned target_directory_length = target_directory.length();

static unsigned file_counter = 0;
constexpr unsigned file_reader_buf_size = 8;

static
//    char
uint8_t
file_buf[file_reader_buf_size];

void forDir(const std::filesystem::directory_entry & start_entry) noexcept {

    ;
    for(std::filesystem::directory_entry it : std::filesystem::directory_iterator(start_entry)) {

        if(!it.exists()) continue;

        if(it.is_directory()) forDir(it);
        else {
            std::cout << "\'" << it.path() << "\' -> ";
                if(it.path().has_extension()) {

                    std::cout << "TYPE: \'" << it.path().extension() << "\'" << std::endl;

                } else {
                    
                    std::ifstream is(it.path().c_str(), std::ios::in | std::ios::binary);
                    std::string file_ext = ".txt";
                    
                    memset(&file_buf[0], 0, file_reader_buf_size);
    //                is.seekg(file_reader_buf_size);
    //                is.readsome((char *)&file_buf[0], file_reader_buf_size);
                    is.read((char *)&file_buf[0], file_reader_buf_size);
                    
//                    for(int i = 0; i< file_reader_buf_size; ++i)
//                        std::cout << "  \'" << file_buf[i] << "\'";
//                    std::cout<<std::endl;
                    
                    {
                        static
                        const
                        std::vector<std::pair<std::string, std::string>> mFormats = {
                            {
                                "\211PNG\r\n\032\n", ".png"
                            },
                            { // next of \340 makes syntax error on xcode(test), ^@^P
                                "\377\330\377\340", ".jpg"
                            },
                            {
                                "\377\330\377\341", ".jpg"
                            },
                            {
                                "ID3", ".mp3"
                            },
                            {
                                "\0\0\0\0ftyp", ".mp4"
                            },
                            {
                                "GIF89a", ".gif"
                            }
                        };
                        
                        for(auto it = mFormats.begin(); it != mFormats.end(); ++it) {
                            if(!std::strncmp((char *)&file_buf[0], it->first.c_str(), std::min<int>(static_cast<int>(it->first.length()), file_reader_buf_size))) {
//                                std::cout << it->second << std::endl;
                                file_ext = it->second;
                                break;
                            }
                        }
                    }

                    std::filesystem::path final_destination = std::filesystem::path(copy_destination_path) / std::filesystem::path(it.path().filename().string() + file_ext);
                    if(std::filesystem::directory_entry(final_destination).exists()) {
                        unsigned cnt = 0;
                        std::filesystem::directory_entry de;
                        do{
                            de = std::filesystem::directory_entry(std::filesystem::path(copy_destination_path) / std::filesystem::path(it.path().filename().string() + "_" + std::to_string(cnt) + file_ext));
                        } while(de.exists());
                        
                        final_destination = de;
                    }

                    std::cout << "\'" << final_destination << "\'...";
                    std::filesystem::copy(it.path(), final_destination);
                    std::cout << "done" << std::endl;
                }
            }

            ++file_counter;
        }

    return;
}

auto main(int argc, char * argv[]) -> int {
    std::ios_base::sync_with_stdio(false); std::cin.tie(NULL); std::cout.tie(NULL);

    { // clean target directory first.
        std::filesystem::path target_directory(copy_destination_path);
        if(std::filesystem::directory_entry(target_directory).exists())
            std::filesystem::remove_all(target_directory);
        std::filesystem::create_directory(target_directory);
    }
    
    auto it = std::filesystem::directory_iterator(target_directory);

    std::cout << it->path() << ": Starting..." << std::endl;

    forDir(*it);

    std::cout << file_counter << std::endl;
    std::cout << ": Finishing..." << std::endl;

    return 0;
}
