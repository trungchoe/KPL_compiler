cmake_minimum_required(VERSION 3.5)

project(scanner)

include_directories()
set(SourceFile scanner.c error.c reader.c token.c charcode.c)

add_executable(scanner ${SourceFile})