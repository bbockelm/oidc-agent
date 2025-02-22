# oidc-agent

oidc-agent is the central comment of the oidc-agent tools. It manages all OpenID
Connect tokens are communicates with the OpenID Providers.
Other applications can request access tokens from the agent.

## Starting oidc-agent
As described in [Xsession integration](configure.md#xsession-integration) on
default oidc-agent is integrated with Xsession. Therefore, it is automatically
started and available in all terminals through that session. So usually a user
does not have to start oidc-agent. 

After installing oidc-agent the agent will not be automatically available. After
a system restart the agent can be used in all terminals.

The agent can also be started by using:
```
oidc-agent
```
This will print out shell commands which have to be executed in the shell where
you want to run oidc-add, oidc-gen, and any application using oidc-agent.

To start oidc-agent and directly set the needed environment variables you can use:
```
eval `oidc-agent`
```

## General Usage
```
Usage: oidc-agent [OPTION...] 
```

See [Detailed Information About All
Options](#detailed-information-about-all-options) for more information.

## Detailed Information About All Options

* [```--confirm```](#-confirm)
* [```--console```](#-console)
* [```--debug```](#-debug)
* [```--kill```](#-kill)
* [```--no-autoload```](#-no-autoload)
* [```--no-webserver```](#-no-webserver)
* [```--no-scheme```](#-no-scheme)
* [```--pw-store```](#-pw-store)
* [```--seccomp```](#-seccomp)
* [```--lifetime```](#-lifetime)
* [```--with-group```](#-with-group)

### ```--confirm```
On default every application running as the same user as the agent can obtain an
access token for every account configuration from the agent. The ```--confirm```
option can be used to change this behavior. If that option is used, the user has
to confirm each usage of an account configuration, allowing fine grained control
from the user. The ```--confirm``` option can be used when loading an account
configuration through ```oidc-add```, in that case only that specific account needs
confirmation, or when starting the agent. If the option is used with the agent,
every usage of every account configuration has to be approved by the user.

### ```--console```
Usually oidc-agent runs in the background as a daemon. This option will skip
the daemonizing and run on the console. This might be sued for debugging.

### ```--debug```
This increases the log level to ```DEBUG``` and obviously should only be used to
debug porpuses. If enabled, sensitive information (among others refresh tokens and client
credentials) are logged to the system log.

### ```--kill```
This will kill the currently running agent. The agent to be killed is identified
by the ```OIDCD_PID``` environment variable. When integrated with Xsession this
will kill the agent available in all terminals. A restarted agent will not
automatically be available in already existing or new terminals. You can use
your [```.bashrc```](configure.md#persisting-oidc-agent-through-bashrc) to make a newly started agent available in new terminals.

### ```--no-autoload```
On default account configurations can automatically be loaded if needed. That means
that an application can request an access token for every account configuration.
If it is not already loaded the user will be prompted to enter the needed
encryption password. After the user enters the password the account configuration
is loaded and an access token returned to the application. the user can also
cancel the autoload.

With ```--no-autoload``` enabled the agent will not load currently not loaded account configuration for which an access token is requested. The user then first has to add them manually by using ```oidc-add```, before an application can obtain an access token for those.

### ```--no-webserver```
This option can be used when the authorization code flow is performed. On default a small
webserver is started by ```oidc-agent``` to be able to catch the redirect and
complete the authorization code flow. The ```--no-webserver``` option tells
```oidc-agent``` that no webserver should be started (for any account
configuration). The authorization code
flow can still be completed. Either by using a redirect uri that follows the
custom redirect uri scheme ```edu.kit.data.oidc-agent:/<path>``` - this will
directly redirect to oidc-gen, or by copying the url the browser would normally
redirect to and pass it to ```oidc-gen --codeExchange```.

### ```--no-scheme
This option can be used when the authorization code flow is performed. The ```--no-scheme``` option tells
```oidc-agent``` that a custom uri scheme should never be used for redirection
(for any account configuration). Normally a custom uri scheme can be used to
redirect direct to (another) oidc-gen instance when performing the
authorization code flow instead of using a web server. However, the redirect to
oidc-gen requires a graphical desktop environment. If this is not present,
redirection with custom uri schemes can be disabled with this option.

### ```--pw-store```
When this option is provided, the encryption password for all account
configurations  will be kept in memory by
oidc-agent (in an encrypted way).

This option can also be sued with ```oidc-add```. When this option is used with
```oidc-agent``` it applies to all loaded account configuration; when used with
```oidc-add``` only for that specific one. See [```oidc-add
--pw-store```](oidc-add.md#-pw-store) for more information.

### ```--seccomp```
Enables seccomp system call filtering. See [general seccomp
notes](security.md#seccomp) for more details.

### ```--lifetime```
The ```--lifetime``` option can be used to set a default lifetime for all loaded account
configurations. This way all account configurations will only be loaded for a
limited time after which they are automatically removed from the agent. 
When loading an account configuration with ```oidc-add``` this lifetime can be
overwritten. So that a specific account configuration can be loaded with another
lifetime (lower, higher, and also infinite).

Using ```--lifetime=0``` means that account configuration are not automatically
removed and they are kept loaded for an infinte time. This is also the default
behavior.

### ```--with-group```
On default only applications that run under the same user that also started the
agent can obtain tokens from it. The `--with-group` option can be used to also
allow other applications access to the agent. This can be useful in cases where
applications must run under a specific user. The user first has to create a
group (e.g. named `oidc-agent`) and add himself and all other users that need
access to the agent to this group. It is the user's responsibility to manage
this group. Then he can pass the group name to the `--with-group` option to
allow all group members access to the agent. If the option is used without
providing a group name, the default is `oidc-agent`.
