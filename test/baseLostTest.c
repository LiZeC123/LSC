int f(char fa,int fb);

int main(){
	char* str = "12343";
	char c = f(str,4);
	return 0;
}

int f(int fa,int fb)
{
	return fa+fb;
}
