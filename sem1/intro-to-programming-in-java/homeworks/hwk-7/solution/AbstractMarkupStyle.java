package markup;

import java.util.List;
import java.util.LinkedList;

/**
 * Abstract class defining the basic structure of most style elements
 *
 * This outlines the implementation for any kind of style element, so that such
 * elements could be implemented in a simpler manner. We can do this because in most markup
 * languages a style is added by first writing an opening style tag, then all of the inner items
 * to which the style should be applied, and then a closing style tag.
 *
 * Because a styled element is still a meaningful element, this implements MarkupItem,
 * meaning that it can be converted to a logical paragraph, just with an extra added style or formatting.
 */
public abstract class AbstractMarkupStyle implements MarkupItem {
    protected List<MarkupItem> elements;

    public AbstractMarkupStyle(List<MarkupItem> elements) {
        this.elements = new LinkedList<>(elements);
    }

    /* Methods which define the required opening and closing tags for the specified markup style */

    public abstract void beginMarkdownStyle(StringBuilder sb);
    public abstract void endMarkdownStyle(StringBuilder sb);

    public abstract void beginBBCodeStyle(StringBuilder sb);
    public abstract void endBBCodeStyle(StringBuilder sb);

    @Override
    public void toMarkdown(StringBuilder sb) {
        beginMarkdownStyle(sb);
        for (MarkupItem element : elements) {
            element.toMarkdown(sb);
        }
        endMarkdownStyle(sb);
    }

    @Override
    public void toBBCode(StringBuilder sb) {
        beginBBCodeStyle(sb);
        for (MarkupItem element : elements) {
            element.toBBCode(sb);
        }
        endBBCodeStyle(sb);
    }

    @Override
    public Paragraph toParagraph() {
        return new Paragraph(List.of(this));
    }
}
