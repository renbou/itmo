package markup;

import java.lang.StringBuilder;

/**
 * Interface defining all kinds of immutable markup classes
 *
 * No matter what a markup class' purpose is, it MUST define these two methods,
 * which are globally used to convert any kind of markup layout to the specified
 * textual representation which might then be used for all kinds of needs.
 *
 * Each method of this interface takes a string builder and fills it with the
 * required format's representation of this element, no matter the element's logical use.
*/
public interface Markup {
    void toMarkdown(StringBuilder sb);
    void toBBCode(StringBuilder sb);
}
