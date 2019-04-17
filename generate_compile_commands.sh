function generate_commands()
{
    cd build
    genie ninja
    ninja -C debug -t compdb cxx > ../compile_commands.json
    cd ..
}

cd flare_math && generate_commands && cd ..
cd flare && generate_commands && cd ..
cd test && generate_commands && cd ..