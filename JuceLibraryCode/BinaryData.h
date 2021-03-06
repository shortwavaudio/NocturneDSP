/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   chunk2_wav;
    const int            chunk2_wavSize = 72104;

    extern const char*   default_wav;
    const int            default_wavSize = 72104;

    extern const char*   model_boost_1202_210922_json;
    const int            model_boost_1202_210922_jsonSize = 101793;

    extern const char*   model_clean_1302_210923_json;
    const int            model_clean_1302_210923_jsonSize = 98231;

    extern const char*   model_crunch_v2_1427_211002_json;
    const int            model_crunch_v2_1427_211002_jsonSize = 98529;

    extern const char*   model_lead_v2_1507_210929_json;
    const int            model_lead_v2_1507_210929_jsonSize = 97991;

    extern const char*   model_rhythm_v2_2009_210925_json;
    const int            model_rhythm_v2_2009_210925_jsonSize = 98810;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 7;

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
