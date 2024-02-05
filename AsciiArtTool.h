//
// Created by noahl on 05/02/2024.
//

#ifndef MATAM_EX1_ASCIIARTTOOL_H
#define MATAM_EX1_ASCIIARTTOOL_H

#include "RLEList.h"

/**
* Ascii Art Tool
*
* Implements an Ascii Art Tool containing a sequence of characters.
*
* The following functions are available:
*   asciiArtRead	        - Reads Ascii Art from a file.
*   asciiArtPrint		    - Prints given Ascii Art in a file.
*   asciiArtPrintEncoded    - Prints given Ascii Art in a file in an encoded way.
*/

/**
* asciiArtRead: Reads Ascii Art from a file.
*
* The function reads characters from a file and returns an RLEList with the characters inside.
* Note that the user must free the returned list using RLEListDestroy() once it is no longer needed.
*
* @return
* 	NULL if allocations failed.
* 	A new RLEList that includes all characters in the file.
*/
RLEList asciiArtRead(FILE* in_stream);

RLEListResult asciiArtPrint(RLEList list, FILE *out_stream);

RLEListResult asciiArtPrintEncoded(RLEList list, FILE *out_stream);

// TO DELETE










#endif //MATAM_EX1_ASCIIARTTOOL_H
