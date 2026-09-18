/* Labb 2 i DD2350 Algoritmer, datastrukturer och komplexitet    */
/* Se labbinstruktionerna i kursrummet i Canvas                  */
/* Ursprunglig författare: Viggo Kann KTH viggo@kth.se           */
import java.util.LinkedList;
import java.util.List;

public class ClosestWords {
  LinkedList<String> closestWords = null;

  int closestDistance = -1;

  int partDist(String w1, String w2, int w1len, int w2len, List<int[]> distanceMatrix) {
    // i is collumn
    for(int i = distanceMatrix.size() ; i <= w2len ; i++) {
      int[] prevColl = distanceMatrix.getLast();
      distanceMatrix.addLast(new int[w1len+1]);
      distanceMatrix.getLast()[0] = i;

      // j is row
      for(int j = 1 ; j <= w1len ; j++) {
        if(w1.charAt(j-1) == w2.charAt(i-1))
          distanceMatrix.getLast()[j]=Math.min(distanceMatrix.getLast()[j-1]+1, Math.min(prevColl[j]+1,prevColl[j-1]));
        else
          distanceMatrix.getLast()[j]=Math.min(distanceMatrix.getLast()[j-1]+1, Math.min(prevColl[j]+1,prevColl[j-1]+1));
      }
    }
    return distanceMatrix.getLast()[w1len];
  }

  int distance(String w1, String w2, List<int[]> distanceMatrix) {
    return partDist(w1, w2, w1.length(), w2.length(), distanceMatrix);
  }

  public ClosestWords(String w, List<String> wordList) {
    int w1len = w.length();

    List<int[]> distanceMatrix = new LinkedList<int[]>();
    distanceMatrix.addLast(new int[w1len+1]);

    for(int i = 0 ; i <= w1len ; i++) {
      distanceMatrix.getFirst()[i] = i;
    }

    String lastWord = "";
    int matchingPrefix = 0;

    for (String s : wordList) {
      matchingPrefix = 0;
      int minLength = Math.min(lastWord.length(),s.length());
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
