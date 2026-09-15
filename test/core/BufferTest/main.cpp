#include <QCoreApplication>
#include <iostream>
#include "../../../src/core/FBuffer.h"


static void printBuf(const char *buf,size_t size ) {
    std::cout << std::endl;
    for(int i = 0; i < size; i++)
    {
        std::cout <<"["<<buf[i]<<"]";
    }
    std::cout << std::endl;
}

static void printVector(const QVector<char>& vec) {
    std::cout << "[";
    for (const auto element :  vec)
        std::cout << element << " ";
    std::cout << "]" << std::endl;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    FRingBuf<char,12> my_buf;
    char display_buf[3];

    my_buf.push('a');
    my_buf.push('b');
    my_buf.push('c');
    my_buf.push('d');
    my_buf.push('e');
    my_buf.push('f');
    my_buf.push('g');
    my_buf.push('h');
    my_buf.push('i');
    my_buf.push('j');
    my_buf.push('k');
    my_buf.push('l');

    my_buf.getWindow(display_buf,3); // retrieve the window once
    printBuf(display_buf,3);

    my_buf.advanceRead(1);
    my_buf.getWindow(display_buf,3);
    printBuf(display_buf,3);

    my_buf.advanceRead(1);
    my_buf.getWindow(display_buf,3);
    printBuf(display_buf,3);

    my_buf.advanceRead(1); //4
    my_buf.getWindow(display_buf,3);
    printBuf(display_buf,3);

    my_buf.push('z');
    my_buf.push('x');
    my_buf.push('w');

    my_buf.advanceRead(1); // after push
    my_buf.getWindow(display_buf,3);
    printBuf(display_buf,3);

    my_buf.push('m');
    my_buf.push('n');
    my_buf.push('o');
    my_buf.getWindow(display_buf,3);
    printBuf(display_buf,3);

    std::cout << "Starting Test 2" << std::endl;
    FRingBuf<char,10> my_buf2;
    my_buf2.push('a');
    my_buf2.push('b');
    my_buf2.push('c');
    my_buf2.push('d');
    my_buf2.push('e');
    my_buf2.push('f');
    my_buf2.push('g');
    my_buf2.push('h');
    my_buf2.push('i');
    my_buf2.push('j');

    QVector<char> out = my_buf2.getRecentWindows(3); //premier cas
    printVector(out);

    my_buf2.push('k');
    my_buf2.push('l');

    out = my_buf2.getRecentWindows(3);//test 2
    printVector(out);

    my_buf2.push('m');
    my_buf2.push('n');

    out = my_buf2.getRecentWindows(3); //test 3
    printVector(out);

    return 0;
}
