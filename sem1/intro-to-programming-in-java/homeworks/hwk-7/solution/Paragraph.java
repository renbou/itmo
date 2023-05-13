package markup;

import java.util.LinkedList;
import java.util.List;

/**
 * A logical paragraph of inner markup items
 *
 * This is the most basic markup block, which logically contains nothing but
 * text and style elements inside of it, joined with some extra formatting if implemented.
 * Due to the fact that this can hold any amount of actual information inside of it,
 * this can form a markup document quite well, which is why it makes sense for it to be
 * a markup block, not an inner element.
 */
public class Paragraph implements MarkupBlock {
    protected List<MarkupItem> elements;

    public Paragraph(List<MarkupItem> elements) {
        this.elements = new LinkedList<>(elements);
    }

    @Override
    public void toMarkdown(StringBuilder sb) {
        for (MarkupItem element : elements) {
            element.toMarkdown(sb);
        }
    }

    @Override
    public void toBBCode(StringBuilder sb) {
        for (MarkupItem element : elements) {
            element.toBBCode(sb);
        }
    }

    @Override
    public MarkupDocument toMarkupDocument() {
        return new MarkupDocument(List.of(this));
    }
}
