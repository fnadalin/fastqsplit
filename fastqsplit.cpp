/*
 ============================================================================
 Name        : fastqsplit.cpp
 Author      : Francesca Nadalin
 Version     : 
 Description : split FASTQ files into chunks of nseq sequences
 ============================================================================
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>

#include <sys/time.h>
#include <sys/resource.h>

auto BUFFER_SIZE = 10000;

int main(int argc, char * argv[]) {
    
    clock_t time1 = clock();

    if (argc < 5)
    {
        std::cout << "Usage: <R1.fastq> <R2.fastq> <nseq> <outdir>" << std::endl;
        return 1;
    }

    std::string R1_file(argv[1]);
    std::string R2_file(argv[2]);
    auto nseq = atoi(argv[3]);
    std::string outdir(argv[4]);

    std::ifstream R1;    
    R1.open(R1_file.c_str());
    if (not R1.is_open())
    {
        std::cerr << "Error while opening file " << R1_file << " for reading" << std::endl;
        return 2;
    }

    std::ifstream R2; 
    R2.open(R2_file.c_str());
    if (not R2.is_open())
    {
        std::cerr << "Error while opening file " << R2_file << " for reading" << std::endl;
        return 2;
    }

    // count the lines
    auto n1 = 0;
    for (std::string line; std::getline(R1, line); ) ++n1;
    R1.close();
    auto n2 = 0;
    for (std::string line; std::getline(R2, line); ) ++n2;
    R2.close();    

    std::cout << "Number of lines: " << n1 << std::endl;

    if (n1 != n2)
    {
        std::cerr << R1_file << " and " << R2_file << " line counts differ" << std::endl;
        return 2;
    }

    if (4*((int) n1/4) != n1) {
        std::cerr << "Line count is not a multiple of 4" << std::endl;
        return 2;
    }

    R1.open(R1_file.c_str());
    R2.open(R2_file.c_str());

    auto c = 0;
    auto n = 0;

    std::ostringstream o1_file, o2_file;
    o1_file << outdir << "/R1_chunk" << c << ".fastq";
    o2_file << outdir << "/R2_chunk" << c << ".fastq";
    std::ofstream o1, o2;
    o1.open(o1_file.str().c_str());
    o2.open(o2_file.str().c_str());

    auto m = 0;
    std::ostringstream buf1, buf2;
    buf1.str("");
    buf2.str("");
    for (std::string line1, line2; std::getline(R1, line1), std::getline(R2, line2); )
    {
        ++m;
        ++n;
        buf1 << line1 << std::endl;
        buf2 << line2 << std::endl;
        if (m == BUFFER_SIZE or n % (4*nseq) == 0 or n == n1) {
            o1 << buf1.str();
            o2 << buf2.str();
            m = 0;
            buf1.str("");
            buf2.str("");
        }
        if (n % (4*nseq) == 0 or n == n1) {   
            o1.close();
            o2.close();
        }
        if (n % (4*nseq) == 0 and n < n1) {
            ++c;
            o1_file.str("");
            o2_file.str("");
            o1_file << outdir << "/R1_chunk" << c << ".fastq";
            o2_file << outdir << "/R2_chunk" << c << ".fastq";
            o1.open(o1_file.str().c_str());
            o2.open(o2_file.str().c_str());
        }
        if (n%100000 == 0)
        {
            std::cout << ".";
            std::cout.flush();
            if (n%10000000 == 0)
            {
                std::cout << std::endl;
            }
        }
    }

    R1.close();
    R2.close();

    double time2 = clock();    
    std::cout << std::endl;
    std::cout << "Wall time: " << (double) (time2 - time1) / CLOCKS_PER_SEC << " s" << std::endl;

    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    std::cout << "RSS: " << usage.ru_maxrss << std::endl;

    return 0;
}

