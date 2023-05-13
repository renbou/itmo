package markup;

import java.util.List;

/**
 * Style element defining stricken-out elements (with a line through them)
 */
public class Strikeout extends AbstractMarkupStyle {
    public Strikeout(List<MarkupItem> elements) {
        super(elements);
    }

    @Override
    public void beginMarkdownStyle(StringBuilder sb) {
        sb.append("~");
    }

    @Override
    public void endMarkdownStyle(StringBuilder sb) {
        sb.append("~");
    }

    @Override
    public void beginBBCodeStyle(StringBuilder sb) {
        sb.append("[s]");
    }

    @Override
    public void endBBCodeStyle(StringBuilder sb) {
        sb.append("[/s]");
    }
}