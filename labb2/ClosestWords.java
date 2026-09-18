/* Labb 2 i DD2350 Algoritmer, datastrukturer och komplexitet    */
/* Se labbinstruktionerna i kursrummet i Canvas                  */
/* Ursprunglig författare: Viggo Kann KTH viggo@kth.se           */
import java.util.LinkedList;
import java.util.ArrayList;
import java.util.List;

public class ClosestWords {
  LinkedList<String> closestWords = null;

  List<int[]> distanceMatrix;

  int closestDistance = -1;

  int partDist(String w1, String w2, int w1len, int w2len, int start) {
      int[] prevRow = distanceMatrix.get(start);
      
    // i is row
    for(int i = start + 1 ; i <= w2len ; i++) {
      int[] newRow = new int[w1len + 1];
      newRow[0] = i;

      // j is col
      for(int j = 1 ; j <= w1len ; j++) {
        if(w1.charAt(j-1) == w2.charAt(i-1))
          newRow[j]=Math.min(newRow[j-1]+1, Math.min(prevRow[j]+1,prevRow[j-1]));
        else
          newRow[j]=Math.min(newRow[j-1]+1, Math.min(prevRow[j]+1,prevRow[j-1]+1));
      }
      prevRow = newRow;
      distanceMatrix.set(i,newRow);

    }
    return distanceMatrix.get(w2len)[w1len];
  }

  int distance(String w1, String w2, int start) {
    return partDist(w1, w2, w1.length(), w2.length(), start);
  }

  public ClosestWords(String w, List<String> wordList) {
    distanceMatrix = new ArrayList<int[]>();

    int wlen = w.length();

    int[] row1 = new int[wlen + 1];

    for(int i = 0 ; i <= wlen ; i++) {
      row1[i] = i;
    }

    distanceMatrix.addFirst(row1);

    String lastWord = "";
    int matchingPrefix = 0;

    for (String s : wordList) {
      
      int rowsToAdd = s.length() - distanceMatrix.size() + 1;
      while (rowsToAdd > 0){
        int[] row = new int[wlen + 1];
        distanceMatrix.add(row);
        rowsToAdd--;
      }

      int minLength = Math.min(lastWord.length(), s.length());

      for(int i = 0; i < minLength; i++){
        if(s.charAt(i) != lastWord.charAt(i)){
          matchingPrefix = i;
          break;
        }
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
    }
  }

  int getMinDistance() {
    return closestDistance;
  }

  List<String> getClosestWords() {
    return closestWords;
  }
}
