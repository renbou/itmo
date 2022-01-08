<#function findBy items kvs...>
    <#list items as item>
        <#list kvs as kv>
            <#if item[kv[0]] == kv[1]>
                <#return [item, item?index]/>
            </#if>
        </#list>
    </#list>
</#function>

<#function cropText text length>
    <#if (text?length > length)>
        <#return text[0..length] + "...">
    <#else>
        <#return text>
    </#if>
</#function>