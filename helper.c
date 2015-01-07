char* char_to_number(unsigned char c){
	static char* fixed = "000";

	unsigned char huns,tens,ones;

	huns = c / 100;
	c = c - huns*100;

	tens = c / 10;
	c = c - tens * 10;

	ones = c;

	fixed[0] = huns+'0';
	fixed[1] = tens+'0';
	fixed[2] = ones+'0';

	return fixed;
}

