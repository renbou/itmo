<#import "util.ftlh" as f>

<#macro displayPost post cropped>
    <#assign u = f.findBy(users, ["id", post.user_id])[0]>
    <article class="post">
        <div class="title">${post.title}</div>
        <div class="information">By ${u.name}</div>
        <div class="body">
            <p>${cropped?then(f.cropText(post.text, 250), post.text)}</p>
        </div>
        <ul class="attachment"></ul>
        <div class="footer">
            <div class="left">
                <img src="/img/voteup.png" title="Vote Up" alt="Vote Up" />
                <span>0</span>
                <img src="/img/votedown.png" title="Vote Down" alt="Vote Down" />
            </div>
            <div class="right">
                <img
                        src="/img/date_16x16.png"
                        title="Publish Time"
                        alt="Publish Time"
                />
                <span>2 days ago</span>
                <img
                        src="/img/comments_16x16.png"
                        title="Comments"
                        alt="Comments"
                />
                <a href="#">0</a>
            </div>
        </div>
    </article>
</#macro>

<#macro navItem items index pointer>
    <#if items[index]??>
        <#nested items index pointer>
    <#else>
        <div>${pointer}</div>
    </#if>
</#macro>

<#macro postlink post text>
    <a href="/post?post_id=${post.id}">${text}</a>
</#macro>

<#macro userlink user text colored>
    <#assign styles = colored?string("color:${user.color};text-decoration:none;", "")>
    <a href="/user?handle=${user.handle}" style="${styles}">${text}</a>
</#macro>

<#function postsByUserId posts userId>
    <#return posts?filter(p -> p.user_id == userId)>
</#function>