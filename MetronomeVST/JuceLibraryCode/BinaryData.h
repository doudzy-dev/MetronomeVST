/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   croches_png;
    const int            croches_pngSize = 181281;

    extern const char*   doubleCroches_png;
    const int            doubleCroches_pngSize = 273058;

    extern const char*   galop_png;
    const int            galop_pngSize = 187457;

    extern const char*   noire_png;
    const int            noire_pngSize = 222705;

    extern const char*   rgalop_png;
    const int            rgalop_pngSize = 188715;

    extern const char*   sextolet_png;
    const int            sextolet_pngSize = 293401;

    extern const char*   triolet_png;
    const int            triolet_pngSize = 184916;

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
