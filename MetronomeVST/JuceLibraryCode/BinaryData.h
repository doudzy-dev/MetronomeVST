/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   croches_png;
    const int            croches_pngSize = 15938;

    extern const char*   doubleCroches_png;
    const int            doubleCroches_pngSize = 2079275;

    extern const char*   galop_png;
    const int            galop_pngSize = 15217;

    extern const char*   noire_png;
    const int            noire_pngSize = 2063046;

    extern const char*   rgalop_png;
    const int            rgalop_pngSize = 15600;

    extern const char*   sextolet_png;
    const int            sextolet_pngSize = 2106036;

    extern const char*   triolet_png;
    const int            triolet_pngSize = 20553;

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
