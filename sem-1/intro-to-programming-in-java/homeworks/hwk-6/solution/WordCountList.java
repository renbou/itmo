public class WordCountList implements Comparable<WordCountList> {
    private final IntList wordIndexes;
    private int totalCount = 0;
    private int lastLine = 0;

    public WordCountList() {
        wordIndexes = new IntList();
    }

    public WordCountList(int line, int... indexes) {
        this();
        totalCount = indexes.length;
        lastLine = line;
        for (int i : indexes) {
            wordIndexes.add(i);
        }
    }

    public IntList getUnderlyingList() {
        return wordIndexes;
    }

    public int getTotalCount() {
        return totalCount;
    }

    public static WordCountList mergeLists(WordCountList a, WordCountList b) {
        if (a.lastLine == b.lastLine) {
            a.totalCount += b.totalCount;
            return a;
        }
        a.lastLine = b.lastLine;
        a.totalCount += b.totalCount;
        a.wordIndexes.addAll(b.wordIndexes);
        return a;
    }

    public int compareTo(WordCountList o) {
        if (totalCount != o.totalCount)
            return totalCount < o.totalCount ? -1 : 1;
        return 0;
    }
}
