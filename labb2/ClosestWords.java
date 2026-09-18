/* Labb 2 i DD2350 Algoritmer, datastrukturer och komplexitet    */
/* Se labbinstruktionerna i kursrummet i Canvas                  */
/* Ursprunglig författare: Viggo Kann KTH viggo@kth.se           */
import java.util.LinkedList;
import java.util.List;

public class ClosestWords {
  LinkedList<String> closestWords = null;

  int closestDistance = -1;

  int partDist(String w1, String w2, int w1len, int w2len, List<int[]> distanceMatrix) {
      int[] prevRow = distanceMatrix.getLast();
      
    // i is row
    for(int i = distanceMatrix.size() ; i <= w2len ; i++) {
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
      distanceMatrix.addLast(newRow);

    }
    return distanceMatrix.getLast()[w1len];
  }

  int distance(String w1, String w2, List<int[]> distanceMatrix) {
    return partDist(w1, w2, w1.length(), w2.length(), distanceMatrix);
  }

  public ClosestWords(String w, List<String> wordList) {
    int wlen = w.length();

    List<int[]> distanceMatrix = new LinkedList<int[]>();
    int[] row1 = new int[wlen + 1];

    for(int i = 0 ; i <= wlen ; i++) {
      row1[i] = i;
    }

    distanceMatrix.add(row1);

    String lastWord = "";
    int matchingPrefix = 0;

    for (String s : wordList) {
      matchingPrefix = 0;
      int minLength = Math.min(lastWord.length(), s.length());
      for(int i = 0; i < minLength; i++){
        if(s.charAt(i) != lastWord.charAt(i)){
          matchingPrefix = i;
          break;
        }
      }

      distanceMatrix = distanceMatrix.subList(0, matchingPrefix+1);

      int dist = distance(w, s, distanceMatrix);
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
