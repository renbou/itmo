package markup;

import java.util.Arrays;
import java.util.List;

/**
 * A list which formats the elements without considering their order in the list
 */
public class UnorderedList extends AbstractMarkupList {
    public UnorderedList(List<ListItem> listElements) {
        super(listElements);
    }

    public UnorderedList(ListItem... listElements) {
        super(listElements);
    }

    @Override
    public void beginBBCodeList(StringBuilder sb) {
        sb.append("[list]");
    }

    @Override
    public void endBBCodeList(StringBuilder sb) {
        sb.append("[/list]");
    }
}
