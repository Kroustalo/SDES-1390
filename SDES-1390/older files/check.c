    if (key > 1023) {
        printf("value is greater than 10 bits");
        return -1;
    }
	
	if (message > 255) {
        printf("value is greater than 8 bits");
        return -1;
    }
	
	if (right > 15) { //saftey
        printf("value is greater than 4 bits");
        return -1;
    }
	
	if (parts[0] > 3) {
        printf("part 1 is greater than 2 bits");
        return -1;
    }
    if (parts[1] > 3) {
        printf("part 2 is greater than 2 bits");
        return -1;
    }