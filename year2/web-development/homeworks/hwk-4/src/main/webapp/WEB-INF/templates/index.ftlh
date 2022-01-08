<#import "lib/common.ftlh" as c>

<@c.page>
    <#list posts?reverse as p>
        <@c.displayPost p true/>
    </#list>
</@c.page>
