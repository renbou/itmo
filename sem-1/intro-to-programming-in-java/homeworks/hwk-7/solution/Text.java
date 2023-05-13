package markup;

import java.util.List;

/**
 * An inner element of markup which simply holds text
 *
 * This is the most basic example of an inner markup item type,
 * which doesn't contain any structural or formatting specifications,
 * just text. However, a document or paragraph might be simply made out of text like
 * this, so it makes sense
 */
public class Text implements MarkupItem {
    protected String text;

    public Text(String text) {
        this.text = text;
    }

    @Override
    public void toMarkdown(StringBuilder sb) {
        sb.append(text);
    }

    @Override
    public void toBBCode(StringBuilder sb) {
        sb.append(text);
    }

    @Override
    public Paragraph toParagraph() {
        return new Paragraph(List.of(this));
    }
}
