<#-- @ftlvariable name="user_id" type="Long" -->
<#import "lib/common.ftlh" as c>

<@c.page>
    <#assign posts = c.postsByUserId(posts, user_id!-1)?reverse>
    <#if posts?size == 0>
        <div>
            <p>
                No posts by such user.
            </p>
        </div>
    <#else>
        <#list posts as p>
            <@c.displayPost p true/>
        </#list>
    </#if>
</@c.page>