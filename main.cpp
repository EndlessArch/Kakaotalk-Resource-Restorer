//
//  main.cpp
//  Test
//
//  Created by Mac on 2020/02/28.
//  Copyright © 2020 Mac. All rights reserved.
//

/*
#include <iostream>
#include <type_traits>

enum class IntersectState {
    NULL_ = 0x00, // can't NULL
    POINT,
    LINE,
    FACE
};

std::string state_to_string(IntersectState _rhs) noexcept {
    if(static_cast<int>(_rhs)) {
        if(static_cast<int>(_rhs) == 1) return "POINT";
        if(static_cast<int>(_rhs) == 2) return "LINE";
        return "FACE";
    }
    return "NULL";
}

class Rect {
    ;
public:
    Rect() = default;

    Rect(int _x1, int _y1, int _x2, int _y2) : x1(_x1), y1(_y1), x2(_x2), y2(_y2) {}

    int x1, y1;
    int x2, y2;

    IntersectState get_intersect_state_with(const Rect & _rhs) const noexcept {
        bool b_xe = (x1 <= _rhs.x1 && _rhs.x1 <= x2) || (x1 <= _rhs.x2 && _rhs.x2 <= x2) || (_rhs.x1 <= x1 && x1 <= _rhs.x2) || (_rhs.x1 <= x2 && x2 <= _rhs.x2);
        bool b_x = (x1 < _rhs.x1 && _rhs.x1 < x2) || (x1 < _rhs.x2 && _rhs.x2 < x2) || (_rhs.x1 < x1 && x1 < _rhs.x2) || (_rhs.x1 < x2 && x2 < _rhs.x2);
        
        bool b_ye = (y1 <= _rhs.y1 && _rhs.y1 <= y2) || (y1 <= _rhs.y2 && _rhs.y2 <= y2) || (_rhs.y1 <= y1 && y1 <= _rhs.y2) || (_rhs.y1 <= y2 && y2 <= _rhs.y2);
        bool b_y = (y1 < _rhs.y1 && _rhs.y1 < y2) || (y1 < _rhs.y2 && _rhs.y2 < y2) || (_rhs.y1 < y1 && y1 < _rhs.y2) || (_rhs.y1 < y2 && y2 < _rhs.y2);

        std::cout << b_xe << b_x << b_ye << b_y << std::endl;
        
        return static_cast<IntersectState>((b_xe && b_ye) * (1 + b_x + b_y));

        if(b_xe) {
            if(!b_x) {
                if(b_ye) {
                    if(!b_y) return IntersectState::POINT;
                    return IntersectState::LINE;
                }
                return IntersectState::NULL_;
            }
            if(b_ye) {
                if(!b_y) return IntersectState::LINE;
                return IntersectState::FACE;
            }
            return IntersectState::NULL_;
        }
        
        return IntersectState::NULL_;
    }

    friend std::istream & operator>>(std::istream &, Rect &) noexcept;

    friend std::ostream & operator<<(std::ostream &, const Rect &) noexcept;
};

std::istream & operator>>(std::istream & _lhs, Rect & _rhs) noexcept {
    int a, b, c, d;
    std::cin >> a >> b >> c >> d;
    _rhs.x1 = a;
    _rhs.y1 = b;
    _rhs.x2 = c;
    _rhs.y2 = d;
    return _lhs;
}

std::ostream & operator<<(std::ostream & _lhs, const Rect & _rhs) noexcept {
    std::cout << _rhs.x1 << " " << _rhs.y1 << " " << _rhs.x2 << " " << _rhs.y2; // << std::endl;
    return _lhs;
}

auto main(int argc, char * argv[]) -> int {

    std::ios_base::sync_with_stdio(false); std::cin.tie(0); std::cout.tie(0);
    
    Rect r1, r2;
    
    std::cin >> r1 >> r2;
    
    std::cout << state_to_string(r1.get_intersect_state_with(r2)) << std::endl;

    return 0;
}
*/

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
    //        else std::bind(applyToFiles, it)();
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
                                "ftyp", ".mp4"
                            }
                        };
                        
                        for(auto it = mFormats.begin(); it != mFormats.end(); ++it) {
                            if(!std::strncmp((char *)&file_buf[0], it->first.c_str(), it->first.length())) {
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

//                    std::cout << "\"";
//                    for(int i = 0; i< file_reader_buf_size; ++i) {
//                        std::cout << (unsigned char)file_buf[i];
////                        if(i != (file_reader_buf_size - 1)) std::cout << ' ';
//                    } std::cout << "\"" << std::endl;
                }
            }

    //        std::cout << std::string(_rhs.path()).substr(target_directory_length) << std::endl;
            ++file_counter;
        }

    return;
}

auto main(int argc, char * argv[]) -> int {
    std::ios_base::sync_with_stdio(false); std::cin.tie(NULL); std::cout.tie(NULL);

    {
        std::filesystem::path target_directory(copy_destination_path);
        if(!std::filesystem::directory_entry(target_directory).exists())
            std::filesystem::create_directory(target_directory);
        else {
            std::filesystem::remove_all(target_directory);
            std::filesystem::create_directory(target_directory);
        }
    }
    
    auto it = std::filesystem::directory_iterator(target_directory);

    std::cout << it->path() << ": Starting..." << std::endl;

    forDir(*it);

    std::cout << file_counter << std::endl;
    std::cout << "ending..." << std::endl;

    return 0;
}
