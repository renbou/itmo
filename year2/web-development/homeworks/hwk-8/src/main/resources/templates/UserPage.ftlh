<#import "commons.ftlh" as c>

<@c.page>
    <div class="user">
        <#if user??>
            <ul>
                <li>id: ${user.id}</li>
                <li>login: ${user.login}</li>
                <li>creationTime: ${user.creationTime}</li>
            </ul>
        <#else>
            No such user.
        </#if>

    </div>
</@c.page>
