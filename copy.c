/**
 * ¹¦ÄÜ£º¿½±´ÎÄ¼þº¯Êý
 * ²ÎÊý£º
 * 		sourceFileNameWithPath£ºÔ´ÎÄ¼þÃû£¨´øÂ·¾¶£©
 * 		targetFileNameWithPath£ºÄ¿±êÎÄ¼þÃû£¨´øÂ·¾¶£©
 * ·µ»ØÖµ£º
 * 		SUCCESS: ¿½±´³É¹¦
 * 		FAILURE£º¿½±´Ê§°Ü
 * author:wangchangshuai jlu
 */
int copyFile(const char *sourceFileNameWithPath,
        const char *targetFileNameWithPath)
{
	FILE *fpR, *fpW;
	char buffer[BUFFER_SIZE];
	int lenR, lenW;
	if ((fpR = fopen(sourceFileNameWithPath, "r")) == NULL)
	{
		myLog("The file '%s' can not be opened! \n", sourceFileNameWithPath);
		return FAILURE;
	}
	if ((fpW = fopen(targetFileNameWithPath, "w")) == NULL)
	{
		myLog("The file '%s' can not be opened! \n", targetFileNameWithPath);
		fclose(fpR);
		return FAILURE;
	}

	memset(buffer, 0, BUFFER_SIZE);
	while ((lenR = fread(buffer, 1, BUFFER_SIZE, fpR)) > 0)
	{
		if ((lenW = fwrite(buffer, 1, lenR, fpW)) != lenR)
		{
			myLog("Write to file '%s' failed!\n", targetFileNameWithPath);
			fclose(fpR);
			fclose(fpW);
			return FAILURE;
		}
		memset(buffer, 0, BUFFER_SIZE);
	}

	fclose(fpR);
	fclose(fpW);
	return SUCCESS;
}
