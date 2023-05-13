import java.io.*;
import java.util.*;

public class ProblemE {
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

    private static IntList[] graph;

    /* Fills the depth array with "depth"/distance to each vert in tree from vert v,
     * fills the parent array with "parents" of each node in the order in which we traversed them in the tree
     */
    private static void bfs(int[] parents, int[] depths, int v) {
        Arrays.fill(depths, -1);
        Queue<Integer> q = new LinkedList<>();

        q.add(v);
        depths[v] = 0;
        parents[v] = -1;
        while (!q.isEmpty()) {
            int cur = q.poll();

            for (Integer to : graph[cur]) {
                if (depths[to] == -1) {
                    q.add(to);
                    depths[to] = depths[cur] + 1;
                    parents[to] = cur;
                }
            }
        }
    }

    public static void main(String[] args) throws IOException {
        BufferedReader stdinReader = new BufferedReader(
                new InputStreamReader(System.in));

        String[] inputStrings = stdinReader.readLine().split(" ");
        int n = Integer.parseInt(inputStrings[0]);
        int m = Integer.parseInt(inputStrings[1]);

        graph = new IntList[n];
        for (int i = 0; i < n; i++) {
            graph[i] = new IntList();
        }

        for(int i = 0; i < n - 1; i++) {
            inputStrings = stdinReader.readLine().split(" ");
            int v = Integer.parseInt(inputStrings[0]);
            int u = Integer.parseInt(inputStrings[1]);

            graph[v - 1].add(u - 1);
            graph[u - 1].add(v - 1);
        }

        int[] teamCities = new int[m];
        inputStrings = stdinReader.readLine().split(" ");
        for (int i = 0; i < m; i++) {
            teamCities[i] = Integer.parseInt(inputStrings[i]) - 1;
        }

        // Trivial case
        if (m < 2) {
            System.out.println("YES");
            System.out.println(1);
            return;
        }

        int[] depths = new int[n];
        int[] parents = new int[n];
        bfs(parents, depths, teamCities[0]);

        int deepestTeam = teamCities[0];

        for (int c : teamCities) {
            if (depths[c] > depths[deepestTeam])
                deepestTeam = c;
        }

        // Go up depth/2 nodes to find the center node between our deepest and first
        int centerNode = deepestTeam;
        for (int i = 0; i < depths[deepestTeam] / 2; i++) {
            centerNode = parents[centerNode];
        }

        depths = new int[n];
        bfs(parents, depths, centerNode);

        boolean goodAnswer = true;
        for (int i = 1; i < m; i++) {
            if (depths[teamCities[0]] != depths[teamCities[i]]) {
                goodAnswer = false;
                break;
            }
        }

        if (goodAnswer) {
            System.out.println("YES");
            System.out.println(centerNode + 1);
        } else {
            System.out.println("NO");
        }
    }
}
