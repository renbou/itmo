package markup;

import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;

/**
 * Abstract class for defining all kinds of lists
 *
 * This outlines the would-be implementation of pretty much any list in markup,
 * so using this allows for easier implementing of lists. We can do this because in pretty much all
 * markdown languages a list consists of opening and closing tags as well as the elements inside of it.
 *
 * Since a list is in itself a meaningful block of information, this is a MarkupBlock,
 * not an inner MarkupItem, and as such this can be used to create a completely valid markup document.
 */
public abstract class AbstractMarkupList implements MarkupBlock {
    protected List<ListItem> listElements;

    public AbstractMarkupList(List<ListItem> listElements) {
        this.listElements = new LinkedList<>(listElements);
    }

    public AbstractMarkupList(ListItem... listElements) {
        this.listElements = Arrays.asList(listElements);
    }


    /* Methods which define the required opening and closing tags for the specified markup style */

    public abstract void beginBBCodeList(StringBuilder sb);
    public abstract void endBBCodeList(StringBuilder sb);

    @Override
    public void toMarkdown(StringBuilder sb) {
        for (ListItem element : listElements) {
            element.toMarkdown(sb);
        }
    }

    @Override
    public void toBBCode(StringBuilder sb) {
        beginBBCodeList(sb);
        for (ListItem element : listElements) {
            element.toBBCode(sb);
        }
        endBBCodeList(sb);
    }

    @Override
    public MarkupDocument toMarkupDocument() {
        return new MarkupDocument(List.of(this));
    }
}
