
/* simple html parser
 * --------------------
 *
 * reads in a html; create a text file;
 * outputs the arduino compatible syntax to the file
 *
 * autumn 2018 | tomek n. (https://cirg.io) for retune festival
 *
 */

void setup() {
  //size(400,200);  // we don't need a canvas

  // load the html input file 
  String[] hmtlFile = loadStrings("sourceLedOn.html"); // file needs to placed in the data folder

  // create a textfile
  PrintWriter output = createWriter("output.txt");

  println("there are " + hmtlFile.length + " lines");
  println();

  // start output string 
  output.print("const char* serverIndex =  \" ");

  // parse the html
  for (int i = 0; i < hmtlFile.length; i++) {
    println("next line: " + hmtlFile[i]);  // just some debug infos for you
    print("char Array: ");
    char [] cArray = hmtlFile[i].toCharArray();
    for (int e=0; e<cArray.length; e++) {
      print(cArray[e]);
      if (cArray[e] == '"') {
        output.print('\'');
      } else {
        output.print(cArray[e]);
      }
    }
    println();
  }

  output.println(" \";");
  output.flush();    // push data into the file
  output.close();    // close file 

  // quit processing
  println();
  println("i'm done, have a nice day");
  exit();
}
