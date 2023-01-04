#pragma once

struct TexCoords
{
    float u;
    float v;
    
    TexCoords(float u, float v);
};

TexCoords GetCoords(char ch);

static const int TexIndex[] =
{
    63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77,                                                // ! " # $ % & ' ( ) * + , - . / 
    54, 55, 56, 57, 58, 59, 60, 61, 62, 63,                                                                    // 0 1 2 3 4 5 6 7 8 9 0
    78, 79, 80, 81, 82, 83, 84,                                                                                // : ; < = > ? @
     1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,    // A B C D E F G H I J K L M N O P Q R S T U V W X Y Z 
    85, 86, 87, 88, 89, 90,                                                                                    // [ \ ] ^ _ ` 
    27, 28, 29, 30, 31, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53,    // a b c d e f g h i j k l m n o p q r s t u v w x y z
    91, 92, 93, 94,                                                                                            // { | } ~
    95, 96                                                                                                     //
};

