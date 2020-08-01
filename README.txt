This is the yangfang first project.
This mode's function is read library url from xml;
functons:
ReadLibraryUrl(vector<string>& LibraryUrls, const QString& fileName);//read file
readHubsurlElement(vector<string>& LibraryUrls); //read root lable 
readUrlElement(vector<string>& LibraryUrls); //read second lable or deep lable
readAddressElement(vector<string>& LibraryUrls); //read url