import java.io.*;
import java.util.*;

public class ProblemM {
    public static class FastScanner implements AutoCloseable {
        public static class MatchGroup {
            private final static int MAX_FASTSET_SIZE = 1<<16;
            public final static int MAX_MATCH_SIZE = Integer.MAX_VALUE;

            private MatchGroup next;

            // Sets to quickly identify most wrong characters
            private final BitSet characterTypeSet;
            private final BitSet characterFastSet;
            // Then check for other characters
            private final Set<Integer> characterSlowSet;

            // If we want any chars that aren't in this group
            private final boolean not;
            // Min and max length of this group, inclusive
            private final int minl, maxl;
            // If we want to match "any" character
            // Actually it will first match when the previous group doesn't match
            // And will stop matching when a character from the next group is found
            // So it's more of like a not(matchGroups[i-1] U matchGroups[i+1])
            // After we create a matchAny group, we MUST set it's next valid/invalid group
            // If the matchAny group is last, and is unlimited, then we will match forever
            private final boolean matchAny;

            public static MatchGroup makeGroup(String chars, byte[] charTypes, boolean not, int atLeast, int noMoreThan, boolean matchAny) {
                return new MatchGroup(chars, charTypes, not, atLeast, noMoreThan, matchAny);
            }

            public MatchGroup(String chars, byte[] charTypes, boolean not, int atLeast, int noMoreThan, boolean matchAny) throws RuntimeException {
                if (atLeast > noMoreThan) {
                    throw new RuntimeException("MatchGroup atLeast bust be <= noMoreThan");
                }
                if (atLeast < 0) {
                    throw new RuntimeException("MatchGroup can match 0 or more, not negative");
                }
                // literally useless group
                if (noMoreThan < 1) {
                    throw new RuntimeException("MatchGroup which matches nothing");
                }
                // if we don't match anything, then uhh die
                if (!matchAny && (chars == null || chars.length() == 0) && (charTypes == null || charTypes.length == 0)) {
                    throw new RuntimeException("Can't create a MatchGroup with nothing in it");
                }
                // we can't match nothing, again...
                if (matchAny && not) {
                    throw new RuntimeException("MatchAny and Not cannot be set at the same time");
                }

                this.not = not;
                this.minl = atLeast;
                this.maxl = noMoreThan;

                // 256 bits will be enough for around 200 years i think,
                // cause currently we only have up to 30 unicode groups
                if (charTypes != null) {
                    characterTypeSet = new BitSet(256);
                    for (byte b : charTypes) {
                        characterTypeSet.set(b);
                    }
                } else {
                    characterTypeSet = null;
                }

                // if we need this group to match absolutely any char, then we don't care about the rest
                if (matchAny) {
                    this.matchAny = true;
                    characterFastSet = null;
                    characterSlowSet = null;
                    return;
                } else {
                    this.matchAny = false;
                }

                // find a perfect fit for the size of the fastset
                if (chars != null) {
                    int maxCharValue = 0, countFits = 0;
                    for (int i = 0; i < chars.length(); i++) {
                        int charCodePoint = chars.codePointAt(i);
                        if (charCodePoint + 1 > maxCharValue && charCodePoint < MAX_FASTSET_SIZE)
                            maxCharValue = charCodePoint + 1;
                        if (charCodePoint < maxCharValue)
                            countFits++;
                    }
                    // if we can't fit everything we need into the fastset, then make a slow one
                    characterFastSet = new BitSet(maxCharValue);
                    if (countFits < chars.length()) {
                        characterSlowSet = new TreeSet<>();
                    } else {
                        characterSlowSet = null;
                    }
                    // and add all the characters we have into it
                    for (int i = 0; i < chars.length(); i++) {
                        int charCodePoint = chars.codePointAt(i);
                        if (charCodePoint < characterFastSet.size()) {
                            characterFastSet.set(charCodePoint);
                        } else {
                            characterSlowSet.add(charCodePoint);
                        }
                    }
                } else {
                    characterFastSet = null;
                    characterSlowSet = null;
                }
            }

            // slow, but we only need to call this once in a lifetime
            public MatchGroup setNext(MatchGroup mg) {
                if (mg == null) {
                    throw new RuntimeException("can't match null group");
                }
                if (matchAny && mg.matchAny) {
                    throw new RuntimeException("can't have two matchesAny groups in a row");
                }
                MatchGroup tmp = this;
                while (tmp.next != null) {
                    tmp = tmp.next;
                }
                tmp.next = mg;
                return this;
            }

            public int shouldAtLeastMatch() {
                return this.minl;
            }

            public int shouldMatchNoMoreThan() {
                return this.maxl;
            }

            public boolean matchesAny() {
                return this.matchAny;
            }

            // Checks if this specific group matches a character
            // Doesn't take MatchAny into account, which is supposed to be handled differently
            public boolean characterMatches(char c) {
                boolean result;
                byte charType = (byte)Character.getType(c);
                // If we have included a whole character group, then simply check if this character fits
                if (characterTypeSet != null && characterTypeSet.get(charType)) {
                    result = true;
                } else {
                    // Otherwise we need to find this specific char
                    // First if we successfully match a character in the fast set
                    // Otherwise check in the slow set
                    result = (characterFastSet != null && characterFastSet.get(c)) ||
                            (characterSlowSet != null && characterSlowSet.contains((int) c));
                }
                if (not)
                    return !result;
                return result;
            }
        }

        // class which we use in order to match a specific list of match groups
        // when it doesn't match, we need to check if we got a full match
        // or if it stopped matching somewhere in the middle, failing
        public static class Matcher {
            // the match groups we must match
            private final MatchGroup matchGroupsIterator;
            private MatchGroup currentGroup;
            // when we match at least minl for match group i,
            // but <= maxl for match group i, then we can start
            // checking if we can match the next group
            // otherwise if >maxl, then we MUST match the next group
            private int currentGroupMatchCnt;
            private int matchLength;

            public Matcher(MatchGroup matchGroupsIterator) {
                if (matchGroupsIterator == null) {
                    throw new RuntimeException("Can't match null list of match groups");
                }
                this.matchGroupsIterator = matchGroupsIterator;
                currentGroup = matchGroupsIterator;
                currentGroupMatchCnt = 0;
                matchLength = 0;
            }

            // resets the matcher
            public void reset() {
                currentGroup = matchGroupsIterator;
                currentGroupMatchCnt = 0;
                matchLength = 0;
            }

            // Matches a group of type "any"
            public boolean matchAny(char c) {
                if (currentGroupMatchCnt >= currentGroup.minl) {
                    MatchGroup nextGroup = currentGroup.next;
                    if (nextGroup == null || !nextGroup.characterMatches(c)) {
                        if (currentGroupMatchCnt == currentGroup.maxl)
                            return false;
                    } else {
                        currentGroup = currentGroup.next;
                        currentGroupMatchCnt = 0;
                    }
                    currentGroupMatchCnt++;
                    matchLength++;
                    return true;
                } else {
                    currentGroupMatchCnt++;
                    matchLength++;
                    return true;
                }
            }

            // This should not be called any more after it returns false
            public boolean matchChar(char c) {
                if (currentGroup.matchAny) {
                    return matchAny(c);
                }

                if (currentGroupMatchCnt < currentGroup.maxl) {
                    boolean currentMatch = currentGroup.characterMatches(c);
                    // if we match, then simply continue
                    if (currentMatch) {
                        matchLength++;
                        currentGroupMatchCnt++;
                        return true;
                    }
                    // otherwise check if we haven't yet matched as much as we need
                    // if we have matched as much as we need, but this group doesn't match, then we will try matching the next one
                    if (currentGroupMatchCnt < currentGroup.minl)
                        return false;
                }

                // if we didn't match this group
                if (currentGroup.next == null) {
                    // if we have matched all groups, then we can't match any more
                    return false;
                }
                // simply try to match for next group
                currentGroupMatchCnt = 0;
                currentGroup = currentGroup.next;
                return matchChar(c);
            }

            // check if we have completely matched all the groups
            // of course you should only call this after matchChar returned false
            public boolean fullMatch() {
                return currentGroup.next == null && currentGroupMatchCnt >= currentGroup.minl;
            }

            // check if we can't match any more after this char
            public boolean finalMatch() {
                return currentGroup.next == null && currentGroupMatchCnt == currentGroup.maxl;
            }

            public int getMatchLength() {
                return matchLength;
            }
        }

        public static final String DEFAULT_DELIMITER_CHARS = "\t\n\u000B\f\r\u001C\u001D\u001E\u001F";
        public final static byte[] DEFAULT_DELIMITER_CHARTYPES =
                new byte[]{Character.LINE_SEPARATOR, Character.PARAGRAPH_SEPARATOR, Character.SPACE_SEPARATOR};
        public static final String DEFAULT_NUMBER_CHARS = "";
        public static final byte[] DEFAULT_NUMBER_CHARTYPES =
                new byte[]{Character.DECIMAL_DIGIT_NUMBER};

        private final static Matcher DEFAULT_DELIMITER_MATCHER = new Matcher(
                new MatchGroup(DEFAULT_DELIMITER_CHARS, DEFAULT_DELIMITER_CHARTYPES,
                        false, 1, MatchGroup.MAX_MATCH_SIZE, false)
        );
        private final static Matcher DEFAULT_STRING_MATCHER = new Matcher(
                new MatchGroup(DEFAULT_DELIMITER_CHARS, DEFAULT_DELIMITER_CHARTYPES,
                        true, 1, MatchGroup.MAX_MATCH_SIZE, false)
        );
        private final static Matcher DEFAULT_NUMBER_MATCHER = new Matcher(
                MatchGroup.makeGroup("-+", null, false, 0, 1, false)
                        .setNext(new MatchGroup(DEFAULT_NUMBER_CHARS, DEFAULT_NUMBER_CHARTYPES,
                                false, 0, MatchGroup.MAX_MATCH_SIZE, false))
        );
        private static Matcher DEFAULT_NEWLINE_MATCHER = new Matcher(
                MatchGroup.makeGroup(null, null, false, 0, MatchGroup.MAX_MATCH_SIZE, true)
                        .setNext(new MatchGroup(String.valueOf("\n"), null, false, 1, 1, false))
        );

        private final static char[] LINE_SEPARATOR = System.lineSeparator().toCharArray();

        // block size is equal to 2^BLOCKP
        private final static int BLOCKP = 16;
        private final static int BLOCKSZ = 1 << BLOCKP;

        private char[] readBuffer;
        private char[] dataBuffer;
        private int bufferSize;
        private int dataStart;
        private int dataEnd;

        private void resizeDataBuffer() {
            char[] copy = new char[dataBuffer.length << 1];
            System.arraycopy(dataBuffer, dataStart, copy, 0, dataEnd - dataStart);
            dataBuffer = copy;
            dataEnd -= dataStart;
            dataStart = 0;
        }

        private void deleteBeginningPart(int length) {
            if (length <= 0)
                return;

            if (length > dataEnd - dataStart)
                throw new RuntimeException("trying to delete more data from beginning than there is");

            dataStart += length;

            if (dataStart > (dataBuffer.length >> 1)) {
                //System.err.println("Moving buffer from " + dataStart +" (size is " +dataBuffer.length+" end is " + dataEnd +")");
                if (!onlyCachedMode)
                    System.arraycopy(dataBuffer, dataStart, dataBuffer, 0, dataEnd - dataStart);
                    // we need to move all the data, not just the fake one
                else
                    System.arraycopy(dataBuffer, dataStart, dataBuffer, 0, realDataEnd - dataStart);
                //System.arraycopy(dataBuffer, dataStart, dataBuffer, 0, dataEnd - dataStart);
                dataEnd -= dataStart;
                dataStart = 0;
            }
        }

        private void readData() {
            if (readerEOF)
                return;
            try {
                int result = 0;
                bufferSize = 0;
                while (bufferSize < BLOCKSZ &&
                        (result = reader.read(
                                readBuffer, bufferSize, BLOCKSZ - bufferSize)) != -1){
                    bufferSize += result;
                }

                if (dataBuffer.length - dataEnd < bufferSize) {
                    resizeDataBuffer();
                }
                System.arraycopy(readBuffer, 0, dataBuffer, dataEnd, bufferSize);
                dataEnd += bufferSize;
                if (result == -1)
                    readerEOF = true;
            } catch (IOException e) {
                readerEOF = true;
            }
        }

        private String dataPrefix(int length) {
            if (dataEnd - dataStart < length) {
                throw new RuntimeException("tried to get prefix of invalid length");
            }
            return String.valueOf(dataBuffer, dataStart, length);
        }

        private Reader reader;
        private boolean readerEOF;

        // the token is [0;endIndex) in the builder
        private int endIndex;
        private int curIndex;
        private boolean useDelimiter = true;

        // charSet is for storing the default matchers in the array of valid matchers
        public enum charSet {DELIMITER, STRING, NUMBER, NEWLINE}
        // the possible matchers, if we want to add a new one,
        // then we just take the next index
        private final static int MAX_MATCHERS = 32;
        private final Matcher[] ValidMatchers;
        private int numMatchers;

        private Object resultCache;
        private int resultCacheType;

        private boolean onlyCachedMode = false;
        private int realEndIndex;
        private int realDataEnd;
        private int realDataStart;

        public FastScanner(Reader r) {
            ValidMatchers = new Matcher[MAX_MATCHERS];
            defaultCharacters();
            reader = r;
            dataBuffer = new char[BLOCKSZ];
            readBuffer = new char[BLOCKSZ];
        }

        // reset the scanner so that you don't have to create
        // all the groups from scratch every time
        public void reset(Reader r) {
            reader = r;
            bufferSize = 0;
            dataStart = 0;
            dataEnd = 0;
            readerEOF = false;
            resultCache = null;
        }

        public void close() throws Exception {
            try {
                reader.close();
            } finally {
                reader = null;
                dataBuffer = null;
                readBuffer = null;
            }
        }

        public Matcher getMatcher(int id) {
            if (id < MAX_MATCHERS)
                return ValidMatchers[id];
            return null;
        }

        public int addMatcher(Matcher matcher) {
            if (numMatchers == MAX_MATCHERS) {
                throw new RuntimeException("can't add any more matchers, there are too many");
            }
            if (matcher == null) {
                throw new RuntimeException("can't add a null matcher to the scanner");
            }
            ValidMatchers[numMatchers] = matcher;
            return numMatchers++;
        }

        private void defaultCharacters() {
            ValidMatchers[charSet.DELIMITER.ordinal()] = DEFAULT_DELIMITER_MATCHER;
            ValidMatchers[charSet.STRING.ordinal()] =  DEFAULT_STRING_MATCHER;
            ValidMatchers[charSet.NUMBER.ordinal()] =  DEFAULT_NUMBER_MATCHER;

            // swap the default matcher for all future scanners
            String lineSep = System.lineSeparator();
            if (!lineSep.equals("\n")) {
                MatchGroup newLineList = new MatchGroup(
                        null, null, false, 0, MatchGroup.MAX_MATCH_SIZE, true);
                for (int i = 0; i < lineSep.length(); i++) {
                    newLineList.setNext(
                            new MatchGroup(String.valueOf(lineSep.charAt(i)), null, false, 1, 1, false)
                    );
                }
                DEFAULT_NEWLINE_MATCHER = new Matcher(newLineList);

            }
            ValidMatchers[charSet.NEWLINE.ordinal()] = DEFAULT_NEWLINE_MATCHER;

            numMatchers = 4;
        }

        // if you want to replace the delimiter for some reason
        // the delimiter MUST be something like [validchars]*
        public void setDelimiter(Matcher delimMatcher) {
            if (delimMatcher == null) {
                throw new RuntimeException("can't set null delimiter list for the scanner");
            }
            ValidMatchers[charSet.DELIMITER.ordinal()] = delimMatcher;
            // Invalidate the cache
            resultCache = null;
        }

        // if we don't care about the delimiter and
        // want to simply find tokens in the text which fit
        public void toggleDelimiter() {
            useDelimiter = !useDelimiter;
            // Invalidate the cache
            resultCache = null;
        }

        public void toggleOnlyCachedMode() {
            if (onlyCachedMode) {
                int moved = realDataStart + realEndIndex - dataEnd;
                dataStart = dataEnd;
                dataEnd = realDataEnd - moved;
//            dataEnd = realDataEnd;
//            dataStart = realEndIndex;
            } else {
                realEndIndex = endIndex;
//            realEndIndex = dataStart + endIndex;
                realDataEnd = dataEnd;
                realDataStart = dataStart;
                dataEnd = dataStart + endIndex;
            }
            onlyCachedMode = !onlyCachedMode;
            // Invalidate the cache
            resultCache = null;
        }

        private char nextChar() {
            if (endIndex != -1)
                return 0;
            // if we need more data
            if (dataStart + curIndex == dataEnd && !onlyCachedMode) {
                readData();
            }
            // we couldn't read another block, so we must've gotten an eof
            if (dataStart + curIndex == dataEnd && (readerEOF || onlyCachedMode)) {
                // here endIndex is only used to mark end of data, so it can be anything else other than -1
                endIndex = 0;
                curIndex++;
                return 0;
            }

            char result = dataBuffer[curIndex + dataStart];
            curIndex++;
            return result;
        }

        private boolean findToken(int tokenType) throws RuntimeException {
            Matcher currentMatcher = getMatcher(tokenType);
            if (currentMatcher == null) {
                throw new RuntimeException(
                        "FastScanner tried hasNextToken with a non-existent type of token, scan would fail"
                );
            }
            currentMatcher.reset();

            Matcher delimiterMatcher = ValidMatchers[charSet.DELIMITER.ordinal()];
            if (delimiterMatcher == null && useDelimiter) {
                throw new RuntimeException(
                        "FastScanner tried hasNextToken until a delimiter with a non-existent " +
                                "delimiter type of token, scan would fail"
                );
            }
            delimiterMatcher.reset();

            // scan until we find a first match
            // if we use a delimiter then only scan delimiter or 0-length line
            // if we don't use a delimiter then just scan until we find any match
            endIndex = -1;
            curIndex = 0;
            char curChar = nextChar();
            if (useDelimiter) {
                while (endIndex == -1 && delimiterMatcher.matchChar(curChar)) {
                    curChar = nextChar();
                }
                // we didn't match the whole delimiter and the string doesn't begin straight away, so no scanning for u
                if (!(curIndex == 1 || delimiterMatcher.fullMatch())) {
                    endIndex = 0;
                    return false;
                }
            } else {
                while (endIndex == -1 && !currentMatcher.matchChar(curChar)) {
                    curChar = nextChar();
                }
                // reset after scanning the useless stuff, cause we have been using it to match
                currentMatcher.reset();
            }

            // remove the garbage after skipping it
            deleteBeginningPart(curIndex - 1);
            curIndex = 1;

            // match the group itself
            while (endIndex == -1 && currentMatcher.matchChar(curChar)) {
                if (!useDelimiter && currentMatcher.finalMatch())
                    break;
                curChar = nextChar();
            }

            // match the end, if we use delimiters then the group must end on a delimiter or eof
            if (!currentMatcher.fullMatch()) {
                endIndex = 0;
                return false;
            }

            // if we finished on an eof, then we don't need to check the delimiter
            if (useDelimiter && endIndex == -1) {
                delimiterMatcher.reset();
                while (endIndex == -1 && delimiterMatcher.matchChar(curChar)) {
                    if (delimiterMatcher.fullMatch())
                        break;
                    curChar = nextChar();
                }
                // didn't match delimiter
                if (!delimiterMatcher.fullMatch()) {
                    endIndex = 0;
                    return false;
                }
            }

            endIndex = currentMatcher.getMatchLength();
            return true;
        }

        public boolean hasNextToken(int tokenType) {
            boolean result = findToken(tokenType);
            if (result) {
                resultCache = dataPrefix(endIndex);
                resultCacheType = tokenType;
            }
            return result;
        }

        public boolean hasNext() {
            return hasNextToken(charSet.STRING.ordinal());
        }

        public boolean hasNextInt() {
            boolean result = findToken(charSet.NUMBER.ordinal());
            if (result) {
                try {
                    resultCache = Integer.valueOf(dataPrefix(endIndex));
                    resultCacheType = charSet.NUMBER.ordinal();
                } catch (NumberFormatException nfe) {
                    return false;
                }
            }
            return result;
        }

        public boolean hasNextLong() {
            boolean result = findToken(charSet.NUMBER.ordinal());
            if (result) {
                try {
                    resultCache = Long.valueOf(dataPrefix(endIndex));
                    resultCacheType = charSet.NUMBER.ordinal();
                } catch (NumberFormatException nfe) {
                    return false;
                }
            }
            return result;
        }

        // faster method instead of pattern searching
        // if you want to specifically match a newline pattern,
        // then replace it to hasNext() with STRING swapped to NEWLINE
        public boolean hasNextLine() {
            endIndex = -1;
            for (int i = 0; endIndex == -1 ; i++) {
                if (i + dataStart == dataEnd) {
                    readData();
                }
                if (i + dataStart == dataEnd) {
                    endIndex = i;
                } else if (dataBuffer[i + dataStart] == LINE_SEPARATOR[0]) {
                    endIndex = i + 1;
                }
            }
            if (endIndex <= 0) {
                return false;
            }
            resultCache = dataPrefix(endIndex);
            resultCacheType = charSet.NEWLINE.ordinal();
            return true;
        }
//    public boolean hasNextLine() {
//        return hasNextToken(charSet.NEWLINE.ordinal());
//    }

        public String nextToken(int tokenType) {
            if (!(resultCacheType == tokenType &&
                    resultCache != null &&
                    resultCache instanceof String)) {
                if (!hasNextToken(tokenType)) {
                    throw new RuntimeException("Tried to get unexistent token");
                }
            }
            String result = (String) resultCache;
            resultCache = null;
            deleteBeginningPart(endIndex);
            return result;
        }

        public String next() {
            return nextToken(charSet.STRING.ordinal());
        }

        public int nextInt() {
            if (!(resultCacheType == charSet.NUMBER.ordinal() &&
                    resultCache != null &&
                    resultCache instanceof Integer)) {
                if (!hasNextInt()) {
                    throw new RuntimeException("Tried to get unexistent int token");
                }
            }
            Integer result = (Integer) resultCache;
            resultCache = null;
            deleteBeginningPart(endIndex);
            return result;
        }

        public long nextLong() {
            if (!(resultCacheType == charSet.NUMBER.ordinal() &&
                    resultCache != null &&
                    resultCache instanceof Long)) {
                if (!hasNextLong()) {
                    throw new RuntimeException("Tried to get unexistent int token");
                }
            }
            Long result = (Long) resultCache;
            resultCache = null;
            deleteBeginningPart(endIndex);
            return result;
        }

        public String nextLine() {
            return nextToken(charSet.NEWLINE.ordinal());
        }
    }

    public static class IntList extends AbstractList<Integer> implements RandomAccess {
        private static final int DEFAULT_CAPACITY = 2;
        private static final int DEFAULT_SHRINK_MINIMUM_CAPACITY = DEFAULT_CAPACITY * 4;

        /* Underlying array used for the list. */
        private int[] array;

        /* We store the currently used size as well as the capacity of the array */
        private int size = 0;
        private int capacity = 0;

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
         * Private functions that are often used for this class' functionality *
         * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

        /* Search for element either from the left or from the right */
        private int find(int value, boolean fromRight) {
            if (!fromRight) {
                for (int i = 0; i < size; i++)
                    if (array[i] == value)
                        return i;
            } else {
                for (int i = size - 1; i >= 0; i--)
                    if (array[i] == value)
                        return i;
            }
            return -1;
        }

        /* Resizes the list if needed. Finds the closest power of 2 that's higher than the requested
         * size in order to maintain amortized O(1) operation time complexity.
         */
        private void optimalResize(int newSize) {
            if (newSize < capacity)
                return;
            while ((newSize & (newSize - 1)) != 0) {
                newSize &= (newSize - 1);
            }
            // if we previously had a null array, just allocate a new one
            if (capacity > 0) {
                array = Arrays.copyOf(array, newSize * 2);
            } else {
                array = new int[newSize * 2];
            }
            capacity = newSize * 2;
        }

        /* Shrinks the underlying when needed in order to save memory space.
         * When the used size is less than one quarter of the capacity, then there's no need for us
         * to keep this much memory, so we can just shrink it. However if the array is too small then there's
         * also no point in constantly calling the gc and allocator, so we define some arbitrary limit.
         */
        private void shrinkIfNeeded() {
            if (capacity > DEFAULT_SHRINK_MINIMUM_CAPACITY && (size < capacity / 4)) {
                capacity /= 2;
                array = Arrays.copyOf(array, capacity);
            }
        }

        /* Removes all elements in the specified range by shifting the rest of the array to their position.
         * This is used by AbstractList for implementing subList and other methods, overriding the base
         * implementation makes it so much faster...
         *s
         * Range is [from; to) just like AbstractList's removeRange
         */
        protected void removeRange(int from, int to) {
            if (to < from || from < 0 || to > size)
                throw new IndexOutOfBoundsException("Can't delete range of elements at invalid position");

            System.arraycopy(array, to, array, from, size - to);
            size -= (to - from);

            shrinkIfNeeded();
        }

        /* Remove element at index, shifting the rest of the array if needed */
        private int removeAt(int index) {
            if (index < 0 || index >= size)
                throw new IndexOutOfBoundsException("Can't delete element at non-existent index");

            int result = array[index];
            if (index < size - 1) {
                System.arraycopy(array, index + 1, array, index, size - index);
            }
            size--;

            shrinkIfNeeded();
            return result;
        }

        /* Insert an element at an arbitrary position of the list, shifting the array elements if needed */
        private void addAt(int index, int element) {
            if (index < 0 || index > size)
                throw new IndexOutOfBoundsException("Can't insert element at negative index or index bigger than current size");

            if (capacity == size) {
                optimalResize(size + 1);
            }
            if (index < size)
                System.arraycopy(array, index, array, index + 1, size - index);
            array[index] = element;
            size++;
        }

        /* * * * * * * * * * * *
         * Simple constructors *
         * * * * * * * * * * * */

        public IntList(int[] array) {
            optimalResize(array.length);
            System.arraycopy(array, 0, this.array, 0, array.length);
            size = array.length;
        }

        public IntList(Integer[] array) {
            optimalResize(array.length);
            for (int i = 0; i < array.length; i++) {
                this.array[i] = array[i];
            }
            size = array.length;
        }

        public IntList(Collection<Integer> c) {
            optimalResize(c.size());
            int i = 0;
            for (Integer item : c) {
                array[i++] = item;
            }
            size = c.size();
        }

        public IntList(int baseArraySize) {
            optimalResize(baseArraySize);
        }

        public IntList() {
            this(DEFAULT_CAPACITY);
        }

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * *
         * The basic functions for interacting with the class  *
         * * * * * * * * * * * * * * * * * * * * * * * * * * * */
        public int size() {
            return this.size;
        }

        public boolean isEmpty() {
            return size == 0;
        }

        public boolean contains(Object o) {
            if (o instanceof Integer) {
                int value = (Integer) o;
                return find(value, false) != -1;
            }
            return false;
        }

        public int indexOf(Object o) {
            if (o instanceof Integer) {
                int value = (Integer) o;
                return find(value, false);
            }
            return -1;
        }

        public int lastIndexOf(Object o) {
            if (o instanceof Integer) {
                int value = (Integer) o;
                return find(value, true);
            }
            return -1;
        }

        public void add(int index, Integer element) {
            addAt(index, element);
        }

        public boolean add(Integer e) {
            addAt(size, e);
            return true;
        }

        public Integer remove(int index) {
            return removeAt(index);
        }

        public boolean remove(Object o) {
            if (o instanceof Integer) {
                int value = (Integer) o;

                int location = find(value, false);

                if (location != -1) {
                    removeAt(location);
                    return true;
                }
            }
            return false;
        }

        public Integer get(int index) {
            if (index < 0 || index >= size)
                throw new IndexOutOfBoundsException("Can't get element at non-existent index");
            return array[index];
        }

        public Integer set(int index, Integer element) {
            if (index < 0 || index >= size)
                throw new IndexOutOfBoundsException("Can't change element at non-existent index");
            Integer previous = array[index];
            array[index] = element;
            return previous;
        }

        /* * * * * * *
         * Iterators *
         * * * * * * */

        public Iterator<Integer> iterator() {
            return new Iterator<>() {
                int index = 0;
                int prevValidIndex = -1;

                @Override
                public boolean hasNext() {
                    return index < size;
                }

                @Override
                public Integer next() {
                    if (index < size) {
                        prevValidIndex = index;
                        return array[index++];
                    }
                    prevValidIndex = -1;
                    throw new NoSuchElementException("Iterator points to end of list, no more elements available");
                }

                @Override
                public void remove() {
                    if (prevValidIndex == -1)
                        throw new IllegalStateException("Can't remove element before next call");
                    removeAt(prevValidIndex);
                    prevValidIndex = -1;
                }
            };
        }

        @Override
        public ListIterator<Integer> listIterator() {
            return listIterator(0);
        }

        public ListIterator<Integer> listIterator(int index_) {
            if (index_ < 0 || index_ >= size)
                throw new IndexOutOfBoundsException("Can't create list iterator which points to an invalid location");
            return new ListIterator<>() {
                int index = index_;
                int prevValidIndex = -1;
                // this is needed to identify where to insert element on "add", there's no good way to avoid this
                boolean prevWasNext = false;

                @Override
                public boolean hasNext() {
                    return index < size;
                }

                @Override
                public Integer next() {
                    if (index < size) {
                        prevValidIndex = index;
                        prevWasNext = true;
                        return array[index++];
                    }
                    prevValidIndex = -1;
                    throw new NoSuchElementException("Iterator points to end of list, no more elements available");
                }

                @Override
                public boolean hasPrevious() {
                    return index > 0;
                }

                @Override
                public Integer previous() {
                    if (index > 0) {
                        index--;
                        prevValidIndex = index;
                        prevWasNext = false;
                        return array[index];
                    }
                    prevValidIndex = -1;
                    throw new NoSuchElementException("Iterator points to end of list, no more elements available");
                }

                @Override
                public int nextIndex() {
                    return index;
                }

                @Override
                public int previousIndex() {
                    return index - 1;
                }

                @Override
                public void remove() {
                    if (prevValidIndex == -1)
                        throw new IllegalStateException("Can't remove element before next or previous call");
                    removeAt(prevValidIndex);
                    prevValidIndex = -1;
                }

                @Override
                public void set(Integer integer) {
                    if (prevValidIndex == -1)
                        throw new IllegalStateException("Can't change element value before next or previous call");
                    array[prevValidIndex] = integer;
                }

                @Override
                public void add(Integer integer) {
                    if (prevValidIndex == -1)
                        throw new IllegalStateException("Can't add element before next or previous call");
                    addAt(prevValidIndex + (prevWasNext ? 0 : 1), integer);
                }
            };
        }

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
         * Collection-based functions which work on multiple elements instead of one *
         * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

        public boolean addAll(Collection<? extends Integer> c) {
            return addAll(size, c);
        }

        public boolean addAll(int index, Collection<? extends Integer> c) {
            if (index > size)
                throw new IndexOutOfBoundsException("Can't add new elements to positions after the end of the list");

            Integer[] cArray = c.toArray(new Integer[0]);
            if (cArray.length == 0)
                return false;

            if (size + cArray.length >= capacity) {
                optimalResize(size + cArray.length);
            }
            System.arraycopy(array, index, array, index + cArray.length, size - index);
            for (int i = 0; i < cArray.length; i++) {
                array[index + i] = cArray[i];
            }
            size += cArray.length;
            return true;
        }

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
         * Static functions for working with lists of this type  *
         * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

        /* The use of these functions is to make this class usable with functions such as
         * Map<T> merge, or make calls to instances of this class chainable etc.
         */

        public static IntList append(IntList integers, Integer value) {
            integers.add(value);
            return integers;
        }

        public static IntList append(IntList integers, IntList integers1) {
            integers.optimalResize(integers.size + integers1.size);
            System.arraycopy(integers1.array, 0, integers.array, integers.size, integers1.size);
            integers.size += integers1.size;
            return integers;
        }

        public static IntList mergeLists(IntList integers, IntList integers1) {
            IntList result = new IntList(integers.size + integers1.size);
            System.arraycopy(integers.array, 0, result.array, 0, integers.size);
            System.arraycopy(integers1.array, 0, result.array, integers.size, integers1.size);
            result.size = integers.size + integers1.size;
            return result;
        }

        public static IntList intListOf(int ...vals) {
            IntList result = new IntList(vals.length);
            for (int v: vals) {
                result.array[result.size++] = v;
            }
            return result;
        }
    }

    public static void main(String[] args) {
        FastScanner stdinScanner = new FastScanner(
                new BufferedReader(new InputStreamReader(System.in)));

        Map<Integer, Integer> fittingK = new HashMap<>();
        int t = stdinScanner.nextInt();

        for (int test_i = 0; test_i < t; test_i++) {
            int n = stdinScanner.nextInt();
            IntList array = new IntList(n);

            for (int i = 0; i < n; i++) {
                array.add(i, stdinScanner.nextInt());
            }
            fittingK.clear();

            int answer = 0;
            fittingK.put(array.get(n-1), 1);
            for (int j = n - 2; j >= 1; j--) {
                for (int i = 0; i < j; i++) {
                    Integer value = fittingK.get(2*array.get(j) - array.get(i));
                    if (value != null) {
                        answer += value;
                    }
                }
                fittingK.merge(array.get(j), 1, Integer::sum);
            }

            System.out.println(answer);
        }
    }
}
