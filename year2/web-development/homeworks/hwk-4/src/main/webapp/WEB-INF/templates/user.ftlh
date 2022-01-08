<#-- @ftlvariable name="handle" type="java.lang.String" -->
<#-- @ftlvariable name="user_id" type="Long" -->
<#import "lib/common.ftlh" as c>

<#macro navUser index pointer>
    <@c.navItem users index pointer>
        <@c.userlink users[index] pointer false/>
    </@c.navItem>
</#macro>

<@c.page>
    <#assign found = c.findBy(users, ["handle", handle!""], ["id", user_id!-1])!["", -1337]/>
    <#assign u = found[0], i = found[1]/>
    <div class="user-info">
        <#if u?has_content>
            <div>
                <div class="name">${u.name}</div>
                <ul class="info-list">
                    <#list ["id", "handle", "name"] as key>
                        <li>${key}: ${u[key]}</li>
                    </#list>
                    <li>
                        <a href="/posts?user_id=${u.id}">
                            Number of posts: ${c.postsByUserId(posts, u.id)?size}
                        </a>
                    </li>
                </ul>
            </div>
            <img src="img/default-user.jpg" class="avatar" alt="avatar"/>
        <#else>
            <p>No such user</p>
        </#if>
    </div>
    <div class="user-navigation">
        <@navUser i-1 "←"/>
        <@navUser i+1 "→"/>
    </div>
</@c.page>
