/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   default_wav;
    const int            default_wavSize = 132404;

    extern const char*   _01_revv_g20_lstm_clean_json;
    const int            _01_revv_g20_lstm_clean_jsonSize = 15583;

    extern const char*   _02_revv_g20_lstm_clean_boost_json;
    const int            _02_revv_g20_lstm_clean_boost_jsonSize = 15694;

    extern const char*   _03_revv_g20_lstm_crunch_json;
    const int            _03_revv_g20_lstm_crunch_jsonSize = 15642;

    extern const char*   _04_revv_g20_lstm_crunch_boost_json;
    const int            _04_revv_g20_lstm_crunch_boost_jsonSize = 15630;

    extern const char*   _05_revv_g20_lstm_rhythm_json;
    const int            _05_revv_g20_lstm_rhythm_jsonSize = 15579;

    extern const char*   _06_revv_g20_lstm_rhythm_boost_json;
    const int            _06_revv_g20_lstm_rhythm_boost_jsonSize = 15558;

    extern const char*   _07_revv_g20_lstm_lead_json;
    const int            _07_revv_g20_lstm_lead_jsonSize = 15637;

    extern const char*   _08_revv_g20_lstm_lead_boost_json;
    const int            _08_revv_g20_lstm_lead_boost_jsonSize = 15569;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 9;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
