int main(int argc, char* str)
{
    char* s = str;
    int i = 0;
    char* argv[3];    

    argv[0] = s;
    while(i<argc) {
        while (*s != '\0')
        {
            ++s;
        }
        argv[i] = s;
        ++i;
    }
}
