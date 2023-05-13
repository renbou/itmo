import java.io.*;
import java.util.*;

public class ProblemK {
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

        public static IntList intListOf(int... vals) {
            IntList result = new IntList(vals.length);
            for (int v : vals) {
                result.array[result.size++] = v;
            }
            return result;
        }
    }

    public static class Rectangle {
        int bottom;
        int top;
        int left;
        int right;

        public Rectangle(int bottom, int top, int left, int right) {
            this.bottom = bottom;
            this.top = top;
            this.left = left;
            this.right = right;
        }
    }

    private final static int A = 0; // just for cleaner code, since we don't need a whole enum

    private static void debugPrintChosenRectangle(int rows, int cols, int[] castlesX, int[] castlesY,
                                                  Rectangle chosenRect, int chosenRectArea) {
        char[][] printArea = new char[rows][cols];
        for (int i = 0; i < rows; i++) {
            Arrays.fill(printArea[i], '.');
        }

        for (int i = chosenRect.bottom + 1; i < chosenRect.top; i++) {
            for (int j = chosenRect.left + 1; j < chosenRect.right; j++) {
                printArea[i][j] = '*';
            }
        }

        for (int i = 0; i < 26; i++) {
            if (castlesX[i] != -1) {
                printArea[castlesX[i]][castlesY[i]] = (char) ('A' + i);
            }
        }

        System.out.print("New rectangle area: ");
        System.out.println(chosenRectArea);
        for (int i = 0; i < rows; i++) {
            System.out.println(printArea[i]);
        }
        System.out.println();
    }

    private static boolean isCastleNotA(char fieldValue) {
        return fieldValue > 'A' && fieldValue <= 'Z';
    }

    public static Rectangle findAreaForKingdomANonBrute(int rows, int columns, char[][] kingdom, int[] castlesX, int[] castlesY) {
        int[] leftBounds = new int[rows];
        int[] rightBounds = new int[rows];

        // Maximum width bounds for rectangles of size 1*m for each row
        for (int i = 0; i < rows; i++) {
            int currentLeft, currentRight;
            for (currentLeft = castlesY[A]; currentLeft >= 0; currentLeft--) {
                if (isCastleNotA(kingdom[i][currentLeft]))
                    break;
            }
            leftBounds[i] = currentLeft;

            for (currentRight = castlesY[A]; currentRight < columns; currentRight++) {
                if (isCastleNotA(kingdom[i][currentRight]))
                    break;
            }
            rightBounds[i] = currentRight;
        }


        Rectangle resultRectangle = new Rectangle(castlesX[A] - 1, castlesX[A] + 1, castlesY[A] - 1, castlesY[A] + 1);
        int maxRectArea = 1;

        // Test all the possible heights, calculating the width for each of them
        int possibleLeftBoundMax = leftBounds[castlesX[A]];
        int possibleRightBoundMax = rightBounds[castlesX[A]];
        for (int bottom = castlesX[A]; bottom >= 0; bottom--) {
            // Update the bounds when we move to the bottom
            if (leftBounds[bottom] > possibleLeftBoundMax)
                possibleLeftBoundMax = leftBounds[bottom];
            if (rightBounds[bottom] < possibleRightBoundMax)
                possibleRightBoundMax = rightBounds[bottom];

            int topPossibleLeftBoundMax = possibleLeftBoundMax;
            int topPossibleRightBoundMax = possibleRightBoundMax;
            for (int top = castlesX[A]; top < rows; top++) {
                topPossibleLeftBoundMax = Math.max(topPossibleLeftBoundMax, leftBounds[top]);
                topPossibleRightBoundMax = Math.min(topPossibleRightBoundMax, rightBounds[top]);

                int currentRectWidth = Math.max(0, topPossibleRightBoundMax - topPossibleLeftBoundMax - 1);
                int currentRectArea = currentRectWidth * (top - bottom + 1);
                if (currentRectArea > maxRectArea) {
                    resultRectangle.left = topPossibleLeftBoundMax;
                    resultRectangle.right = topPossibleRightBoundMax;
                    resultRectangle.top = top + 1;
                    resultRectangle.bottom = bottom - 1;
                    maxRectArea = currentRectArea;

                    //debugPrintChosenRectangle(rows, columns, castlesX, castlesY,
                    //    resultRectangle, maxRectArea);
                }
            }
        }

        return resultRectangle;
    }

    private static void nonOptimizingSolve1(int rows, int columns, char[][] answer) {
        // Sizes of kingdom columns
        int[][] bottom = new int[rows][columns];
        int[][] top = new int[rows][columns];

        // Fill column to the top
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                if (isCastleNotA(answer[i][j])) {
                    bottom[i][j] = i;
                    for (int len = 1; i - len >= 0 && answer[i - len][j] == '.'; len++) {
                        bottom[i][j] = i - len;
                        answer[i - len][j] = Character.toLowerCase(answer[i][j]);
                    }
                }
            }
        }

        // Fill column to the bottom
        for (int i = 0; i < rows; i++) {
            for (int j = columns - 1; j >= 0; j--) {
                if (isCastleNotA(answer[i][j])) {
                    top[i][j] = i;
                    for (int len = 1; i + len < rows && answer[i + len][j] == '.'; len++) {
                        top[i][j] = i + len;
                        answer[i + len][j] = Character.toLowerCase(answer[i][j]);
                    }
                }
            }
        }

        // Fill column to the left
        for (int j = 0; j < columns; j++) {
            for (int i = 0; i < rows; i++) {
                if (isCastleNotA(answer[i][j])) {
                    for (int leftSide = j - 1; leftSide >= 0; leftSide--) {
                        boolean canFillColumn = true;
                        for (int k = bottom[i][j]; k <= top[i][j]; k++) {
                            if (answer[k][leftSide] != '.') {
                                canFillColumn = false;
                                break;
                            }
                        }

                        if (!canFillColumn)
                            break;

                        for (int k = bottom[i][j]; k <= top[i][j]; k++) {
                            answer[k][leftSide] = Character.toLowerCase(answer[i][j]);
                        }
                    }
                }
            }
        }

        // Fill column to the right
        for (int j = columns - 1; j >= 0; j--) {
            for (int i = 0; i < rows; i++) {
                if (isCastleNotA(answer[i][j])) {
                    for (int rightSide = j + 1; rightSide < columns; rightSide++) {
                        boolean canFillColumn = true;
                        for (int k = bottom[i][j]; k <= top[i][j]; k++) {
                            if (answer[k][rightSide] != '.') {
                                canFillColumn = false;
                                break;
                            }
                        }

                        if (!canFillColumn)
                            break;

                        for (int k = bottom[i][j]; k <= top[i][j]; k++) {
                            answer[k][rightSide] = Character.toLowerCase(answer[i][j]);
                        }
                    }
                }
            }
        }
    }

    private static void nonOptimizingSolve2(int rows, int columns, char[][] answer) {
        // Left and right row bounds for each kingdom
        int[][] left = new int[rows][columns];
        int[][] right = new int[rows][columns];

        // Fill row to the left
        for (int j = 0; j < columns; j++) {
            for (int i = 0; i < rows; i++) {
                if (isCastleNotA(answer[i][j])) {
                    left[i][j] = j;
                    for (int width = 1; j - width >= 0 && answer[i][j - width] == '.'; width++) {
                        left[i][j] = j - width;
                        answer[i][j - width] = Character.toLowerCase(answer[i][j]);
                    }
                }
            }
        }

        // Fill row to the right
        for (int j = 0; j < columns; j++) {
            for (int i = 0; i < rows; i++) {
                if (isCastleNotA(answer[i][j])) {
                    right[i][j] = j;
                    for (int width = 1; j + width < columns && answer[i][j + width] == '.'; width++) {
                        right[i][j] = j + width;
                        answer[i][j + width] = Character.toLowerCase(answer[i][j]);
                    }
                }
            }
        }

        // Fill row downwards
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                if (isCastleNotA(answer[i][j])) {
                    for (int bottomSide = i - 1; bottomSide >= 0; bottomSide--) {
                        boolean canFillRow = true;
                        for (int k = left[i][j]; k <= right[i][j]; k++) {
                            if (answer[bottomSide][k] != '.') {
                                canFillRow = false;
                                break;
                            }
                        }

                        if (!canFillRow)
                            break;

                        for (int k = left[i][j]; k <= right[i][j]; k++) {
                            answer[bottomSide][k] = Character.toLowerCase(answer[i][j]);
                        }
                    }
                }
            }
        }

        // Fill row upwards
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                if (isCastleNotA(answer[i][j])) {
                    for (int topSide = i + 1; topSide < rows; topSide++) {
                        boolean canFillRow = true;
                        for (int k = left[i][j]; k <= right[i][j]; k++) {
                            if (answer[topSide][k] != '.') {
                                canFillRow = false;
                                break;
                            }
                        }

                        if (!canFillRow)
                            break;

                        for (int k = left[i][j]; k <= right[i][j]; k++) {
                            answer[topSide][k] = Character.toLowerCase(answer[i][j]);
                        }
                    }
                }
            }
        }
    }

    public static boolean answerIsOk(char[][] answer, int rows, int columns) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                if (answer[i][j] == '.' || answer[i][j] == 0) {
                    return false;
                }
            }
        }
        return true;
    }

    public static char[][] firstSolution(int rows, int columns, char[][] kingdom) {
        int[] castlesX = new int[26];
        Arrays.fill(castlesX, -1);
        int[] castlesY = new int[26];
        Arrays.fill(castlesY, -1);

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                if (Character.isLetter(kingdom[i][j])) {
                    int currentKingdom = kingdom[i][j] - 'A';
                    castlesX[currentKingdom] = i;
                    castlesY[currentKingdom] = j;
                }
            }
        }

        Rectangle kingdomARect = findAreaForKingdomANonBrute(rows, columns, kingdom, castlesX, castlesY);
        // Create the answer solely for kingdom A
        char[][] answerA = new char[rows][columns];
        for (int i = 0; i < rows; i++) {
            System.arraycopy(kingdom[i], 0, answerA[i], 0, columns);
        }
        for (int i = kingdomARect.bottom + 1; i < kingdomARect.top; i++) {
            for (int j = kingdomARect.left + 1; j < kingdomARect.right; j++) {
                answerA[i][j] = 'a';
            }
        }
        answerA[castlesX[A]][castlesY[A]] = 'A';

        // Prepare two possible answers
        char[][] answer1 = new char[rows][columns];
        char[][] answer2 = new char[rows][columns];
        for (int i = 0; i < rows; i++) {
            System.arraycopy(answerA[i], 0, answer1[i], 0, columns);
            System.arraycopy(answerA[i], 0, answer2[i], 0, columns);
        }

        nonOptimizingSolve1(rows, columns, answer1);
        nonOptimizingSolve2(rows, columns, answer2);

        if (answerIsOk(answer1, rows, columns))
            return answer1;
        if (answerIsOk(answer2, rows, columns))
            return answer2;

        System.err.println("Answer 1:");
        for (int rw = 0; rw < rows; rw++) {
            System.err.println(answer1[rw]);
        }

        System.err.println("Answer 2:");
        for (int rw = 0; rw < rows; rw++) {
            System.err.println(answer2[rw]);
        }

        throw new RuntimeException("Can't solve current problem");
    }

    public static void stressTest(int tests) {
        Random rng = new Random(1);
        for (int i = 0; i < tests; i++) {
            int rows = rng.nextInt(21) + 1;
            int columns = rng.nextInt(21) + 1;
            System.out.printf("Test %d:\n", i);
            System.out.printf("\tRows = %d, Columns = %d\n", rows, columns);
            int numCastles = rng.nextInt(Math.min(rows * columns, 26)) + 1;

            char[][] kingdom = new char[rows][columns];
            for (int rw = 0; rw < rows; rw++) {
                Arrays.fill(kingdom[rw], '.');
            }

            int[] castlesX = new int[numCastles];
            int[] castlesY = new int[numCastles];

            for (int castle = 0; castle < numCastles; castle++) {
                boolean locationGenerated = false;
                while(!locationGenerated) {
                    locationGenerated = true;
                    castlesX[castle] = rng.nextInt(rows);
                    castlesY[castle] = rng.nextInt(columns);

                    for (int otherCastle = 0; otherCastle < numCastles; otherCastle++) {
                        if (castle == otherCastle)
                            continue;
                        if (castlesX[castle] == castlesX[otherCastle] && castlesY[castle] == castlesY[otherCastle]) {
                            locationGenerated = false;
                            break;
                        }
                    }
                }
                kingdom[castlesX[castle]][castlesY[castle]] = (char)('A' + castle);
            }

            System.out.println("Kingdom:");
            for (int rw = 0; rw < rows; rw++) {
                System.out.println(kingdom[rw]);
            }
            System.out.println("Answer:");

            char[][] answer = firstSolution(rows, columns, kingdom);
            // Print the answer
            for (int rw = 0; rw < rows; rw++) {
                System.out.println(answer[rw]);
            }

            if (!answerIsOk(answer, rows, columns)) {
                throw new RuntimeException("Invalid answer generated");
            }
        }
    }

    public static void main(String[] args) throws IOException {
        //stressTest(100);

        BufferedReader stdinReader = new BufferedReader(new InputStreamReader(System.in));

        String[] firstParams = stdinReader.readLine().split(" ");
        int rows = Integer.parseInt(firstParams[0]);
        int columns = Integer.parseInt(firstParams[1]);

        char[][] kingdom = new char[rows][];
        for (int i = 0; i < rows; i++) {
            String current = stdinReader.readLine();
            kingdom[i] = current.toCharArray();
        }

        char[][] answer = firstSolution(rows, columns, kingdom);

        // Print the answer
        for (int i = 0; i < rows; i++) {
            System.out.println(answer[i]);
        }
    }
}
