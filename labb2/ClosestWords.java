/* Labb 2 i DD2350 Algoritmer, datastrukturer och komplexitet    */
/* Se labbinstruktionerna i kursrummet i Canvas                  */
/* Ursprunglig författare: Viggo Kann KTH viggo@kth.se           */
import java.util.LinkedList;
import java.util.List;

public class ClosestWords {
  LinkedList<String> closestWords = null;

  int[][] dMat = new int[50][50];

  int closestDistance = -1;

  int partDist(String w1, String w2, int w1len, int w2len, int start) {
      
    // i is row
    for(int i = start ; i <= w2len ; i++) {
      dMat[i][0] = i;

      // j is col
      for(int j = 1 ; j <= w1len ; j++) {
        if(w1.charAt(j-1) == w2.charAt(i-1))
          dMat[i][j]=Math.min(dMat[i][j-1]+1, Math.min(dMat[i - 1][j]+1,dMat[i - 1][j-1]));
        else
          dMat[i][j]=Math.min(dMat[i][j-1]+1, Math.min(dMat[i - 1][j]+1,dMat[i - 1][j-1]+1));
      }

    }
    return dMat[w2len][w1len];
  }

  int distance(String w1, String w2, int start) {
    return partDist(w1, w2, w1.length(), w2.length(), start);
  }

  public ClosestWords(String w, List<String> wordList) {


    for(int i = 0 ; i < 50 ; i++) {
      dMat[0][i] = i;
    }


    String lastWord = "";
    int matchingPrefix = 1;

    for (String s : wordList) {
      
      int minLength = Math.min(lastWord.length(), s.length());

      for(int i = 0; i < minLength; i++){
        if(s.charAt(i) == lastWord.charAt(i)){
          matchingPrefix++;
        } else { break; }
      }

       
      
      int dist = distance(w, s, matchingPrefix);
      // System.out.println("d(" + w + "," + s + ")=" + dist);
      if (dist < closestDistance || closestDistance == -1) {
        closestDistance = dist;
        closestWords = new LinkedList<String>();
        closestWords.add(s);
      }
      else if (dist == closestDistance)
        closestWords.add(s);
      lastWord = s;
      matchingPrefix = 1;
    }
  }

  int getMinDistance() {
    return closestDistance;
  }

  List<String> getClosestWords() {
    return closestWords;
  }
}
