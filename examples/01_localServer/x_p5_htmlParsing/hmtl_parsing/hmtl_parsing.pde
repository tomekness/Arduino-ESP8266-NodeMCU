
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
  String[] hmtlFile = loadStrings("source.html"); // file needs to placed in the data folder

  // create a textfile
  PrintWriter output = createWriter("output.txt");

  println("there are " + hmtlFile.length + " lines");
  println();
  
  // first things first
  output.println("  // Return the response");
  output.println("  client.println(\"HTTP/1.1 200 OK\");");
  output.println("  client.println(\"Content-Type: text/html\");");
  output.println("  client.println(\"\"); //  do not forget this one)");
  
  // parse the html
  for (int i = 0; i < hmtlFile.length; i++) {
    println(hmtlFile[i]);  // just some debug infos for you
    output.println("  client.println(\"" + hmtlFile[i] + "\");");  // wirte the file
  }
  
  output.flush();    // push data into the file
  output.close();    // close file 

  // quit processing
  println();
  println("i'm done, have a nice day");
  exit();
}
