<#-- @ftlvariable name="post_id" type="Long" -->
<#import "lib/common.ftlh" as c>

<@c.page>
    <#assign found = c.findBy(posts, ["id", post_id!-1])!["", -1337]/>
    <#assign p = found[0], i = found[1]/>
    <#if p?has_content>
        <@c.displayPost p false/>
    <#else>
        <div><p>No such post</p></div>
    </#if>
</@c.page>