
#include "Tests.h"

std::array<Juse::U8, 256> Juse::Tests::higherLowerGame()
{
    return {
        0x00, 0x01, 0, 93, // goto @main
        // @messagePrompt [4]
        'H', 'i', 'g', 'h', 'e', 'r', '/', 'l', 'o', 'w', 'e', 'r', ' ', 'g', 'a', 'm', 'e', ':',
        ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // #DB~36

        // @more [40]
        'H', 'i', 'g', 'h', 'e', 'r', '\n', 0, 0, 0, 0, 0, // #DB~12

        // @less [52]
        'L', 'o', 'w', 'e', 'r', '\n', 0, 0, 0, 0, 0, 0, 0, // #DB~13

        // @victoryMsg [65]
        'C', 'o', 'n', 'g', 'r', 'a', 't', 'u', 'l', 'a', 't', 'i', 'o', 'n', 's', '!', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // #DB~26

        // @number [91]
        0, 0, // #DW

        // @main [93]
        0x14, 0x00, 0, 0, 0, // Words[0] = 0
        0x14, 0x00, 1, 0x27, 0x0f, // Words[1] = 9999
        0x15, 0x06, 2, 0, 1, // Words[2] = {rnd Words[0] Words[1]}
        0x14, 0x02, 0, 91, 2, // [@number] = Words[2]

        // @loop [113]
        0x00, 0x04, 0, 151, // call @prompt
        0x14, 0x01, 1, 0, 91, // Words[1] = [@number]
        0x15, 0xf0, 0, 1, // Words[0] ? Words[1]
        0x00, 0x08, 0, // If =
        0x00, 0x01, 0, 160, // goto @victory
        0x00, 0x08, 2, // If <
        0x13, 0x10, 0, 40, // out S8 @more
        0x00, 0x08, 1, // If >
        0x13, 0x10, 0, 52, // out S8 @less
        0x00, 0x01, 0, 113, // goto @loop

        // @prompt [151]
        0x13, 0x10, 0, 4, // out S8 @messagePrompt
        0x17, 0x01, 0, // in Words[0]
        0x00, 0x05, // return

        // @victory [160]
        0x13, 0x10, 0, 65, // out S8 @victoryMsg
        0x00, 0x0f, // end
    };
}
